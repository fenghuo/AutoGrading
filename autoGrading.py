import ConfigParser
import cStringIO
import difflib
import email
import glob
import json
import logging
import logging.handlers
import os
import re
import select
import shutil
import signal
import smtplib
import stat
import sys
import tempfile
import time
import subprocess
import datetime
import csv
import copy
import xml.etree.ElementTree as ET
from os.path import isdir, join
from optparse import OptionGroup, OptionParser
from subprocess import Popen, PIPE, STDOUT,call

class Shell:
	@staticmethod
	def call(command, stdin=None,stdout=None,stderr=PIPE, timeout=1,shell=False):
		start = datetime.datetime.now()
		process = subprocess.Popen(command, stdin=stdin, stdout=stdout, stderr=stderr,shell=shell)
		while process.poll() is None:
			time.sleep(0.1)
			now = datetime.datetime.now()
			if (now - start).seconds> timeout:
				os.kill(process.pid, signal.SIGKILL)
				os.waitpid(-1, os.WNOHANG)
				print "Execution Timeout!"
				return 408
		if(process.stderr!=None):
			temp=(''.join(process.stderr.readlines())).strip()
			if(temp!=""):
				print temp
		return process.returncode
	
	@staticmethod
	def diff(fp1,fp2):
		r1=''.join(fp1.readlines())
		r2=''.join(fp2.readlines())
		t1=""
		t2=""
		for t in r1:
			if(t=='-' or (t>='0' and t<='9')):
				t1+=t
		for t in r2:
			if(t=='-' or (t>='0' and t<='9')):
				t2+=t
		return t1==t2
			

class TestCase:
	def __init__(self,ID,testIn,testResult,score):
		self.testID=ID
		self.testIn=testIn
		self.testResult=testResult
		self.score=score
		self.result=CaseResult(ID,1,"",score)
		self.timeout=1
		
	def load():
		print "loading test case: "+testID	
		
	def gen(self,command):
		testID=self.testID
		print "generating test cases: "+str(testID)
		tin=open(self.testIn,"r")
		tout=open(testID+".output","wr")
		tcode=open(testID+".c","wr")
		Shell.call(["make"])
		Shell.call(command,stdin=tin,stdout=tout,stderr=tcode,timeout=5)
		tin.close()
		tout.close()
		tcode.close()


	def run(self,command,log,error):
		testID=self.testID
		self.result=CaseResult(testID,1,"",self.score)
		print "running test cases: "+str(testID)
		tin=open(self.testIn,"r")
		tout=open(testID+".output","wr")
		tcode=open(testID+".c","wr")
		result=Shell.call(command,stdin=tin,stdout=tout,stderr=tcode)
		if(result==408):
			self.result.error+=" Execution Timeout!"		
		tin.close()
		tout.close()
		tcode.close()

class TestP0Scan(TestCase):
	def gen(self,command):
		TestCase.gen(self,command)
		
		tout=open(self.testID+".output","r").readlines()
		errorLine=""

		for line in reversed(tout):
			if(line.find("scan error")>=0):
				errorLine=line[line.find('line')+4:].strip()
				break
		return errorLine

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

		toutt=open(self.testID+".output","r")
		tout=toutt.readlines()
		toutt.close()
		tresultt=open(self.testResult,"r")
		tresult=tresultt.readlines()
		tresultt.close()
		lineCounter=""
		lineExpect=''.join(tresult)
		
		for line in reversed(tout):
			if(line.find("scan error")>=0):
				lineCounter=line[line.find('line')+4:]				
				break
		if(lineCounter.strip()==lineExpect.strip()):
			self.result.result=0
		else:
			self.result.error+="Scanner Error! Expect Error Line: "+str(lineExpect).strip()+" ,your error line: "+str(lineCounter).strip()+"! "
			self.result.result=1
		if(self.result.result==0):
			self.result.error=""
		return self.result

class TestP0Parse(TestCase):
	def gen(self,command):
		
		TestCase.gen(self,command)

                tout=open(self.testID+".output","r")
		result=[]
		errorLine=None
		for line in tout:
			if(line.find("scan error")>=0):
				print "Find scan error!----------"
				return
			if(line.find("syntax error")>=0):
				errorLine=line[line.find('line')+4:].strip()
				break;

			if(line.find('fillcolor=powderblue')>=0):
				for c in line:
					if(c>='0' and c<='9'):
						break;
					elif(c=='"' or c=='\t' or c==' '):
						continue
					else:
						result.append(c)
			
		result=''.join(result).strip()	
		if(errorLine!=None):
			result=errorLine
		tout.close()
		return result


	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

                tout=open(self.testID+".output","r").readlines()
		result=[]
		errorLine=None
		for line in tout:
			if(line.find("syntax error")>=0):
				errorLine=line[line.find('line')+4:].strip()
				break;

			if(line.find('fillcolor=powderblue')>=0):
				for c in line:
					if(c>='0' and c<='9'):
						break;
					elif(c=='"' or c=='\t' or c==' '):
						continue
					else:
						result.append(c)
			
		tresultt=open(self.testResult,"r")
		tresult=tresultt.readlines()
		tresultt.close()
		result=''.join(result).strip()	
		tresult=''.join(tresult).strip()
		if(errorLine!=None):
			result=errorLine
		if(result==tresult):
			self.result.result=0
		else:
			self.result.error+="Parse Tree Error! "	
			self.result.result=1
		if(self.result.result==0):
			self.result.error=""
		return self.result

class TestP0Code(TestCase):
	def gen(self,command):
		TestCase.gen(self,command)

		testID=self.testID
		tcoutput=open(testID+".c.output","wr")
		result=0
		result=Shell.call(["gcc","-lm",testID+'.c',"-o",testID])
		if(result!=0):
			print "C code Compile error"
		
		result=Shell.call(["./"+testID],stdout=tcoutput)
		if(result!=0):
			print "C code runtime error"
		
		tcoutput.close()	
		tcoutput=open(testID+".c.output","r+b")
		return ''.join(tcoutput.readlines()).strip()
		

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

		testID=self.testID.strip()
		tcoutput=open(testID+".c.output","wr")
		result=0
		tempfile="temp.log."+testID
		Shell.call(["rm","-f",tempfile,testID])
		temp=open(tempfile,"wb")
		result=Shell.call(["gcc","-lm",testID+'.c',"-o",testID],stdout=log,stderr=temp)
		temp.close()
		if(result!=0):
			try:
				temp=''.join(open(tempfile).readlines())
				if(temp.find("error:")>=0):
					self.result.error+="C code Compile error! "
			except:
				None	
		try:
			result=Shell.call(["./"+testID],stdout=tcoutput,stderr=error)
			if(result!=0):
				self.result.error+="C code runtime error! "
	
			#result=Shell.call(["diff","-bBiw",self.testResult,testID+".c.output"],stdout=log,stderr=error)
			fp1=open(self.testResult)
			fp2=open(testID+".c.output")
			if(Shell.diff(fp1,fp2)):
				result=0
			else:
				result=103	
			fp1.close()
			fp2.close()
			if(result!=0):
				self.result.error+="C code execution output error! "
			
			if(result==408):
				self.result.error+="Execution Timeout!"		
				
			self.result.result=result
		except:
			self.result.error+="Run time error! "
			self.result.result=9

		tcoutput.close()
		if(self.result.result==0):
			self.result.error=""
		return self.result

class CaseResult:
	def __init__(self,ID,result,error,score):
		self.ID=ID
		self.result=result
		self.error=error
		self.score=score

class Result:
	def __init__(self):
		self.score=0
		self.result={}

	def load(self,filePath):
		fp=open(filePath,"rb")
		

class Test:
	def __init__(self):
		self.testCases=[]
		self.result=None
	
	def loadTest():
		print "loading tests:"

	def addTest(self,ID,testIn,testResult,score,level):
		
		if(level=='0.1'):
			self.testCases.append(TestP0Scan(ID,testIn,testResult,score))
		elif(level=='0.2'):
			self.testCases.append(TestP0Parse(ID,testIn,testResult,score))
		elif(level=='0.3'):
			self.testCases.append(TestP0Code(ID,testIn,testResult,score))
		else:
			print "Unkown test type"

	def genOutput(self,command):
		result=[]
		print "generating outputs:"	
		for case in self.testCases:
			result.append(case.run(command))
		result.score=self.getScore()
		return result
	
	def runTest(self,command,log,error):
		self.result=Result()
		Shell.call(["cat","README"])
		Shell.call(["make","clean"],timeout=5)
		Shell.call(["rm","-f","calc"])
		Shell.call("make",timeout=5)
		build=True
		try:
			open("calc").close()
		except:
			build=False
		print "running test:"	
		for case in self.testCases:
			if(build):
				self.result.result[case.testID]=case.run(command,log,error)
			else:
				self.result.result[case.testID]=CaseResult(case.testID,101,"Fail to build, no target!",0)
		self.result.score=self.getScore()
		return self.result
		
	def getScore(self):
		score=0
		for case in self.testCases:
			r=self.result.result[case.testID]
			if(r.result==0):
				score+=int(case.score)
		return score
		
			
class Student:
	def __init__(self,alias,first,last,email):
		self.name=alias
		self.testResult=None
		self.first=first
		self.last=last
		self.email=email
		self.graded=False
		self.submitted=False
		self.test=Test()

	def grade(self,command,log,error):
		print "grading: "+self.name
		self.testResult=copy.deepcopy(self.test.runTest(command,log,error))
		self.graded=True

	def dump(self,fp):
		fp.write(self.alias+"\t\t")
		for case in self.result.result:
			fp.write(case.result+"\t")
		fp.write("\n")

	def email(self):
		fp = open(textfile, 'rb')
		# Create a text/plain message
		msg = MIMEText(fp.read())
		fp.close()
		
		# me == the sender's email address
		# you == the recipient's email address
		msg['Subject'] = 'The contents of %s' % textfile
		msg['From'] = me
		msg['To'] = you
		
		# Send the message via our own SMTP server, but don't include the
		# envelope header.
		s = smtplib.SMTP('localhost')
		s.sendmail(me, [you], msg.as_string())
		s.quit()
	
	def save(self,filePath):
		fp = open(filePath, 'wb')
		fp.write("<html><body border='1'>\n")	
		fp.write("<h1>CS160: The report of project 0 grading:</h1><h1> Name: "+self.name+"</h1>\n")
		fp.write("<h3> Total Score: "+str(self.testResult.score)+" / 100 </h3>\n")	
		fp.write("<table><tr><th>ID</th><th>Result</th><th>Error Message</th></tr>\n")
		result=self.testResult.result
		for case in self.test.testCases:
			fp.write("<tr>\n")
			fp.write("<td>"+str(case.testID)+"</td>\n")
			rs="Pass"
			if(result[case.testID].result!=0):
				rs="Fail"
			fp.write("<td>"+str(rs)+"</td>\n")
			fp.write("<td>"+str(result[case.testID].error)+"</td>\n")
			fp.write("</tr>")
		fp.write("</table>\n")	
		fp.write("</body></html>")
		fp.close()
		
class Grading:
	def __init__(self):
		#self.workDir="/cs/student/tianjiu/Documents/ta/cs160/proj0/calc2/"
		self.configPath="/cs/class/cs160/grading/AutoGrading/"
		self.workDir="/cs/class/cs160/grading/AutoGrading/source/"
		self.students={}
		self.test=Test()
		test=""
		self.testPath=self.configPath+"testcase.xml"
		self.testInput=self.configPath+"testinput.xml"
		self.testDir=self.configPath+"testcases/"
		self.proj="proj0"


		self.log = open("log","w")
		self.error = open("error","w")
		
	def __del__(self):
		self.log.close()
		self.error.close()

	def loadStudentList(self,filePath):
		try:
			fp=open(filePath,"rb+")
		except:
			print "Student List doesn't exist!"
			return 
		self.students.clear()
		for line in fp.readlines():
			st=line.split('\t')
			self.students[st[0]]=Student(st[0],st[1],st[2],st[3])	

		fp.close()

	def genStudentResult(self,filePath):
		fp=open(filePath+".back","wb+")
		fp.write("Alias\tLast Name\tTotal Score\t")
		for case in self.test.testCases:
			fp.write(str(case.testID)+"\tError\t")
		fp.write("\n")
		for alias in sorted(self.students.keys()):
			stu=self.students[alias]
			fp.write(alias+"\t"+stu.last+"\t")
			if(stu.graded):		
				fp.write(str(stu.testResult.score)+"\t")
				for case in self.test.testCases:
					r=stu.testResult.result[case.testID]
					fp.write(str(r.result)+"\t"+r.error+"\t")
			else:
				fp.write("None")
			fp.write("\n")

		fp.close()
		Shell.call(["rm","-f",filePath])
		Shell.call(["cp",filePath+".back",filePath])

	def loadStudentResult(self,filePath):
		if(len(self.students.keys())<1):
			print "No Student Loaded"
			return
		try:
			fp=open(filePath,"rb+")
		except:
			print "Student Result doesn't exist!"
			return 
		keys=fp.readline().strip().split('\t')
		for line in fp.readlines():
			print line
			items=line.split('\t')
			stu=self.students[items[0].strip()]
			print items[0]+"\t"+str(stu.graded)
			stu.testResult=Result()
			try:
				stu.testResult.score=int(items[2].strip())
			except:
				print "-"+items[2]+"-"
				stu.graded=False
				continue
			for i in range(3,len(keys),2):
				ID=keys[i]
				stu.testResult.result[ID]=CaseResult(ID,int(items[i]),items[i+1],0)
			stu.graded=True
	
	def printStudentList(self):
		for key in self.students.keys():
			print key+" -- "+self.students[key].email
		print len(self.students.keys())

	def runShell(self,command):
		self.log.write(time.strftime('%X %x %Z')+'\n')
		self.log.write("command: "+ ' '.join(command)+'\n')
		self.error.write(time.strftime('%X %x %Z')+'\n')
		self.error.write("command: "+ ' '.join(command)+'\n')
		self.log.flush()
		self.error.flush()
		return Shell.call(command,stdout=self.log,stderr=self.error)

	def genTestOutput(self,command,sPath):
		print "generating all the answers of test cases:\n"
		sys.stdout.flush()
		os.chdir(sPath);	
		print "Working Directory: "+os.getcwd()
		Shell.call(["rm","-f",self.testPath])
		Shell.call(["rm","-rf",self.testDir])
		Shell.call(["mkdir",self.testDir])		
		tree = ET.parse(self.testInput)
		root = tree.getroot()
		test=Test()
		output=open(self.testPath,"wb")
		output.write("<data>\n")
		for testcase in root:
			ID=testcase[0].text
			Tin=testcase[1].text
			Tscore=testcase[2].text
			Ttype=testcase[3].text
			
			Tinp=self.testDir+ID+'.in'
			Tfin=open(Tinp,"wb")
			if(Tin==None):
				Tin=""
			Tfin.write(Tin)
			Tfin.close()

			test.addTest(ID,Tinp,None,Tscore,Ttype)
			print "----------   "+ID
			Tout=test.testCases[len(test.testCases)-1].gen(command)
			print Tout
	
			output.write("\t<test>\n")
			output.write("\t\t<ID>"+ID +"</ID>\n")
			output.write("\t\t<Input>"+Tin +"</Input>\n")
			output.write("\t\t<Output>"+Tout +"</Output>\n")
			output.write("\t\t<Score>"+Tscore +"</Score>\n")
			output.write("\t\t<Type>"+Ttype +"</Type>\n")
			output.write("\t</test>\n")

		output.write("</data>\n")
			
		output.close()
	
	def getStudentName(self):
		fr=open("studentList","rb+")
		fw=open("studentName","wb")
		for alias in fr.readlines():
			alias=alias.strip()
			process = subprocess.Popen(["finger",alias],stdout=PIPE)
			temp=process.stdout.read()	
			name=temp[temp.find("Name:")+5:temp.find("Directory")].strip()
			lastName=name[name.rfind(" "):]
			firstName=name[0:name.find(" ")]
			print lastName+" --- "+ alias+"\t"+name+"\n"	
			fw.write(lastName+"\t"+alias+"\t"+firstName+"\n")
		fr.close()
		fw.close()
		

	def genTestCase(self):
		print "generating all test cases:\n"
		os.chdir(self.configPath);	
		Shell.call(["rm","-rf",self.testDir])
		Shell.call(["mkdir",self.testDir])		
		tree = ET.parse(self.testPath)
		root = tree.getroot()
		self.test=Test()
		for testcase in root:
			ID=testcase[0].text
			Tin=testcase[1].text
			Tout=testcase[2].text
			Tscore=testcase[3].text
			Tlevel=testcase[4].text

			Tinp=self.testDir+ID+'.in'
			Tfin=open(Tinp,"wb")
			if(Tin==None):
				Tin=""
			Tfin.write(Tin)
			Tfin.close()

			Toutp=self.testDir+ID+'.result'
			Tfout=open(Toutp,"wb")
			if(Tout==None):
				Tout=""
			Tfout.write(Tout)
			Tfout.close()

			self.test.addTest(ID,Tinp,Toutp,Tscore,Tlevel)

	def genStudentSubmission(self):
		print "decompressing students' projects"
		Shell.call(["rm","-r",self.workDir+self.proj+"/extract"])
		Shell.call(["mkdir",self.workDir+self.proj+"/extract"])
		os.chdir(self.workDir+self.proj)
		print "working directory: \n"+os.getcwd()
		for alias in self.students.keys():
			print "extracting "+ alias
			fileName=alias+".tar.Z"
			Shell.call(["rm","-r",alias])
			Shell.call(["mkdir",alias])
			for i in range(1,1000):
				newfileName=alias+"-"+str(i)+".tar.Z"
				try:
					open(newfileName).close()	
				except:
					break
				fileName=newfileName
			print fileName
			Shell.call(["mkdir","extract/"+alias])
			Shell.call(["tar","-zxvf",fileName,"-C","extract/"+alias])
			if(not os.path.isdir("extract/"+alias+"/calc")):
				Shell.call(["mkdir","extract/"+alias+"/calc"])
				Shell.call(["tar","-zxvf",fileName,"-C","extract/"+alias+"/calc"])
			
	def testGrade(self):
		
		Shell.call(["clear"])
		print "Testing grading for "+self.proj
		self.genTestCase()
		#path="/cs/class/cs160/project0/solution/"
		path="/cs/class/cs160/grading/AutoGrading/source/proj0/extract/htl/calc"
		os.chdir(path)
		print "Working directory: "+os.getcwd()
		result=self.test.runTest(["./calc"],self.log,self.error)
		for key in sorted(result.result.keys()):		 
			print key+ "\t"+str(result.result[key].result)
			if(result.result[key].result!=0):
				print "\t"+result.result[key].error
		print result.score
		
		#s=Student("Keith")
		#s.testResult=result
		#s.save("Report.htm")

	def startGrading(self):
		Shell.call("clear")
		#print "start grading"
		#os.chdir(self.workDir);	
		#print "working directory: \n"+os.getcwd()
		#self.log = open("log","w")
		#self.error = open("error","w")
		#self.runShell(["make","clean"])
		#if(self.runShell(["make"])!=0):
		#	print "compile error"
		#	return
	
		#case=TestP0Scan("1","1.in","1.result",1)
		#case=TestP0Parse("1","1.in","1.result",1)
		#case=TestP0Code("1","1.in","1.result",1)
		#print case.gen(["./calc"])
		#print case.errorMsg
		#print case.testScan("","")

		self.genTestCase()
		
		self.loadStudentList(self.configPath+"studentFullList")
		self.loadStudentResult(self.configPath+"proj0.result")

		for alias in self.students.keys():
			Shell.call("clear")
			print "Grading: "+alias
			stu=self.students[alias]
			print stu.graded
			if(stu.graded):
				continue
			os.chdir(self.workDir+self.proj+"/extract/"+alias+"/calc/")
			print "Working directory: "+os.getcwd()
			#self.test.runTest("./calc",self.log,self.error)
			stu.test=self.test
			stu.grade(["./calc"],self.log,self.error)
			self.genStudentResult(self.configPath+"proj0.result")	
			stu.save(self.configPath+"result/"+alias+".report.htm")


		#result=self.test.runTest(["./calc"],self.log,self.error)
		#for r in result.result:		
		#	print r.ID
		#	print "\t"+str(r.result)
		#	if(r.result!=0):
		#		print "\t"+r.error
		#print result.score
		#
		#s=Student("Keith")
		#s.testResult=result
		#s.save("Report.htm")
		#for student in self.students:
		#	student.grade(["./calc"],self.log,self.error)
		#	print student.name+" "+student.score

def main():
	os.chdir(sys.path[0])

	parser = OptionParser()
	parser.add_option('--grade')
	options, args = parser.parse_args()

	if options.grade:
		print "grading:"

	else:
		print "No Options!"





proj0=Grading()
proj0.genTestOutput(["./calc"],"/cs/class/cs160/project0/solution/")
proj0.genTestCase()
#proj0.getStudentName()
#proj0.loadStudentList(proj0.configPath+"studentFullList")
#proj0.genStudentSubmission()
#proj0.genStudentResult(proj0.answerPath+"proj0.result")
proj0.startGrading()
proj0.genStudentResult("proj0.result.xls")
#proj0.testGrade()
if __name__ == '__main__':
    sys.exit(main())
