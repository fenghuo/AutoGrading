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
from random import shuffle
import xml.etree.ElementTree as ET
from os.path import isdir, join
from optparse import OptionGroup, OptionParser
from subprocess import Popen, PIPE, STDOUT,call
from email.mime.text import MIMEText

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
		self.purpose="Others"
		
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
		result=Shell.call(command,stdin=tin,stdout=tout,stderr=tcode,timeout=20)
		if(result==408):
			self.result.error+=" Execution Timeout!"		
		tin.close()
		tout.close()
		tcode.close()

	def getInput(self):
		tin=open(self.testIn,"r")
		temp=""
		count=1
		for t in tin.readlines():
			temp+=str(count)+": "+t
			count=count+1
		tin.close()
		return temp

class TestP0Scan(TestCase):

	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		self.purpose="Scanner Fully Working"

	def gen(self,command):
		TestCase.gen(self,command)
		
		tout=open(self.testID+".output","r").readlines()
		errorLine=""

		for line in reversed(tout):
			if(line.find("scan error")>=0):
				errorLine=line[line.rfind('line')+4:].strip()
				break
		return errorLine

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

		if(self.result==408):
			return

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
	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		if(ID[:1]=="1"):
			self.purpose="Parser Fully Works on Correct Input"
		else:
			self.purpose="Parser Throws Correct Errors"

	def gen(self,command):
		
		TestCase.gen(self,command)

		if(self.result==408):
			return

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
				errorLine=line[line.rfind('line')+4:].strip()
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
			if(result.strip("eof").strip("EOF")==tresult):
				print "Has eof trailing in parse, others are correct\n"
				self.result.result=0
			else:
				self.result.error+="Parse Tree Error! "	
				self.result.error+="Expected: " + tresult + ", Found: " + result
				self.result.result=1
		if(self.result.result==0):
			self.result.error=""
		return self.result

class TestP0Code(TestCase):
	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		self.purpose="Program Emits Correct C Code"

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

		if(self.result==408):
			return

		testID=self.testID.strip()
		tcoutput=open(testID+".c.output","wr")
		result=0
		tempfile="temp.log."+testID
		Shell.call(["rm","-f",tempfile,testID])
		temp=open(tempfile,"wb")
		result=Shell.call(["gcc","-lm",testID+'.c',"-o",testID],stdout=log,stderr=temp)
		#result=Shell.call(["g++",testID+'.c',"-o",testID],stdout=log,stderr=temp)
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

class TestP1(TestCase):
	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		self.purpose="Scanner and Parser Fully Works"

	def gen(self,command):
		TestCase.gen(self,command)

		if(self.result==408):
			return

                tout=open(self.testID+".c","r")
		result=""
		for line in tout:
			if(line.find("syntax error")>=0):
				result=line[line.rfind('line')+4:].strip()
				break;
		tout.close()
		return result

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

		if ( self.testID=="601" ):
			return self.runComment(command,log,error)

                tout=open(self.testID+".c","r").readlines()
		result=""
		for line in tout:
			if(line.find("syntax error")>=0):
				result=line[line.rfind('line')+4:].strip()
				break;

		tresultt=open(self.testResult,"r")
		tresult=tresultt.readlines()
		tresultt.close()
		tresult=''.join(tresult).strip()

		if(result==tresult):
			self.result.result=0
		else:
			self.result.error+="Compiler parses incorrectly! "	
			self.result.error+="Expected: " + tresult + ", Found: " + result
			self.result.result=1
		if(self.result.result==0):
			self.result.error=""
		return self.result
	
	def runComment(self,command,log,error):
		
                tout=open(self.testID+".c","r").readlines()
		result=""
		for line in reversed(tout):
			if(line.find("line")>=0):
				result=line[line.rfind('line')+4:].strip()
				break;

		self.result.result=1
		self.result.error+="Dangling Comment Error!"
		try:
			if(int(result)>=27):
				self.result.result=0
				self.result.error=""
		except:
			self.result.result=1
		return self.result

class TestP2(TestCase):
	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		self.purpose="Generate Correct ast"

	def gen(self,command):
		TestCase.gen(self,command)
		result=""
		fmt="[label="
                tout=open(self.testID+".output","r").readlines()
		for line in tout:
			if(fmt in line):
				result+=TestP2.pro(self,line[line.rfind(fmt)+len(fmt)+1:line.rfind("\"")])+"\n"
		return result 

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

                tout=open(self.testID+".output","r").readlines()
		error=""
		fmt="[label="
		result=[]
		for line in tout:
			if(line.find("syntax error")>=0):
				error=line[line.rfind('line')+4:].strip()
				break;
			if(fmt in line):
				result.append(TestP2.pro(self,line[line.rfind(fmt)+len(fmt)+1:line.rfind("\"")])+"\n")

		tw=open(self.testID+".output2","w")
		tw.write(''.join(result))
		tw.close()	
		tresultt=open(self.testResult,"r")
		tresult=tresultt.readlines()
		tresultt.close()
	
		if(error!="" or len(result)==0):
	                self.result.result=1
			self.result.error="Runtime Error! "+error 
			print self.result.error
			return self.result
	
		dis=TestP2.distance(self,result,tresult)	
	
		print "Distance : " +str(dis)
		r=0
		if(dis==0):
			r=0
		elif(dis<10):
			r=0.1
		elif(dis<20):
			r=0.2
		elif(dis<30):
			r=0.3
		elif(dis<50):
			r=0.4
		elif(dis<70):
			r=0.5
		elif(dis<90):
			r=0.6
		elif(dis<120):
			r=0.7
		elif(dis<150):
			r=0.8
		elif(dis<200):
			r=0.9
		else:
			r=1
		self.result.result=r
		if(dis!=0):
			self.result.error="AST mismatch!"
		return self.result

	def distance(self,s1,s2):
	        l1=len(s1)
       		l2=len(s2)
		print l1
		print l2
        	res = [[0 for x in xrange(l2+1)] for x in xrange(l1+1)]
        	for x in xrange(l1):
               		res[x][l2]=l1-x
        	for x in xrange(l2):
                	res[l1][x]=l2-x
        	for x in reversed(xrange(l1)):
                	for y in reversed(xrange(l2)):
                        	if(s1[x]==s2[y]):
                                	res[x][y]=res[x+1][y+1]
                        	else:
                                	res[x][y]=min(res[x+1][y],res[x][y+1])+1
        	return res[0][0]

	def pro(self,s):
		s=s.replace("\\n","_")
		s=s.replace("\\\"","")
		s=s.replace(" ","")
		return s

class TestP3(TestCase):

	typeErrors=[
		"error: no Program class",
		"error: no start function in Program class",
		"error: start function has arguments",
		"error: duplicate identifier name in same scope",
		"error: symbol by name undefined",
		"error: symbol by name defined, but of unexpected type",
		"error: function call has different number of args than the declaration",
		"error: type mismatch in function call args",
		"error: type mismatch in return statement",
		"error: types of right and left hand side do not match in assignment",
		"error: predicate of if statement is not boolean",
		"error: incompatible types used in expression",
		"error: function doesn't exist in object"		
		]

	def __init__(self,ID,testIn,testResult,score):
		TestCase.__init__(self,ID,testIn,testResult,score)
		self.purpose="Generate Correct Type Checking Error"

	def gen(self,command):
		TestCase.gen(self,command)
		result=""
		fmt="on line number"
                tout=open(self.testID+".output","c").readline()
		if(tout.find(fmt)>=0):
			result=tout[tout.find(fmt)+len(fmt):tout.find(",")].strip()
		return result 

	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

                tout=open(self.testID+".c","r").readline()
		error=""
		etype=""
		enum=""
		eno=int(str(self.testID)[0:-2])
		fmt="on line number"
		if(tout.find(fmt)>=0):
			enum=tout[tout.find(fmt)+len(fmt):tout.find(",")]
			etype=tout[tout.rfind(",")+1:]

		tresultt=open(self.testResult,"r")
		tresult=tresultt.readline()
		tresultt.close()

		r=1.0
		if(enum.strip()==tresult.strip()):
			r-=0.2
		else:
			error+="\tExpect error line: "+tresult.strip()+", yours: "+enum.strip()
		if(etype.strip()==self.typeErrors[eno]):
			r-=0.8
		else:
			error+="\tExpect :"+self.typeError[eno]+",yours: "+etype.strip()
		self.result.result=r
		self.result.error=error
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
		elif(level=='1.0' or level=='1.1'):
			self.testCases.append(TestP1(ID,testIn,testResult,score))
		elif(level=='2.0'):
			self.testCases.append(TestP2(ID,testIn,testResult,score))
		elif(level=='3.0'):
			self.testCases.append(TestP3(ID,testIn,testResult,score))
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
		Shell.call("make",timeout=60)
		build=True
		try:
			open(command[0][2:]).close()
		except:
			build=False
		print "running test:"	
		for case in self.testCases:
			if(build):
				self.result.result[case.testID]=case.run(command,log,error)
			else:
				self.result.result[case.testID]=CaseResult(case.testID,1,"Fail to build, no target!",0)
		self.result.score=self.getScore()
		return self.result
		
	def getScore(self):
		score=0
		for case in self.testCases:
			r=self.result.result[case.testID]
			score+=int(case.score)*(1-r.result)
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

	def checkPenalty(self,penalty=None):
		
		if(self.name in penalty):
			self.testResult.score-=penalty[self.name]
		#adjust the score
		s=int(self.testResult.score)
		if(s>5):
			s=s*0.7+30		
		self.testResult.score=int(s)


	def sendResult(self,From="cs160@cs.ucsb.edu",To=None,proj="proj",penalty=None,median=0,average=0,stdev=0):
		if(To==None):
			To=self.email
		print "To :"+To

		pmsg=""
		#if(self.name in penalty):
			#pmsg="PS: You were taken "+str(penalty[self.name])+" points as penalty for following reasons:\n\tYou didn't submit project correctly as instructed\n\n"
		#	self.testResult.score-=penalty[self.name]
	
		lmsg=""
		if(int(self.testResult.score)<=5):
			lmsg+="You may have scored lower than you expected. The two reasons why projects received a zero score are:\n     1. The project did not compile, even after we investigated.\n     2. The grammar does not parse correct programs, and therefore the project never generated any ASTs for any input.\nPlease come see us to discuss your project, so that you can solve Project 4.\n\n"
	
		msg="Hi "+self.first+",\n\n"
		msg+="For the project "+str(proj)+", you got "+str(self.testResult.score)+" / 100.\n\n"
		msg+="FYI, some statistics was shown below.\n"
		msg+="\t\tMedian:              "+str(median)+"\n"
		msg+="\t\tAverage:             "+str(average)+"\n"
		msg+="\t\tStandard Deviation:  "+str(stdev)+"\n\n"
		msg+="THIS EMAIL ADDRESS IS NOT MONITORED, PLEASE DO NOT REPLY HERE. Send any and all emails to the TAs directly and do not email anything to cs160@cs.ucsb.edu. Thank you!\n\n"
		msg+="This score is your INITIAL project grade. If you think your project was scored incorrectly, you may come meet with a TA before the end of next Tuesday (3/18) and we will discuss your grade. If you do meet with us, we will assign a FINAL project grade after the discussion.\n\n"
		msg+="Project 2 is designed as a basis for the subsequent projects, and we want to make sure you have every opportunity to solve it correctly. If you need help solving your project 2 errors, please meet with a TA by appointment or during office hours\n\n"
		msg+="A selection of test case inputs on which your program failed are attached. If you have any questions, you are welcome to email the TAs.\n\n"
		msg+=pmsg
		msg+=lmsg
		msg+="Best,\nCS160 TAs\n\n"
		msg+="-------------------- Selected Test Case Inputs -----------------\n\n"
		count=1	
		result=self.testResult.result
		for case in (self.test.testCases): 
			if(result[case.testID].result!=0):
				if(count>3):
					msg+=" ... \n"
					break
				msg+="No. "+str(count)+"\n"	
				msg+="Test Case Purpose: "+case.purpose+"\n"
				msg+="Error Message captured: "+result[case.testID].error+"\n\n"	
				msg+=" _____________Start of Test Case "+case.testID+"____________\n"
				msg+=case.getInput()
				msg+="\n______________End of Test Case___________________\n\n"
				count=count+1


		msg = MIMEText(msg)
		msg['Subject'] = "CS160: Results of Project"+str(proj)
		msg['From'] = From
		msg['To'] = To
		back="tianjiu.yin@gmail.com"
		
		s = smtplib.SMTP('stamps.cs.ucsb.edu', 587)
		try:
			print "Sending email to "+self.name
			s.sendmail(From, [To], msg.as_string())
			#s.sendmail(From, [back],msg.as_string())
		except:
			print "Send email to "+self.name+" Failed!"
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
	def __init__(self,proj=0,folder="calc",command=["./calc"]): 
		#self.workDir="/cs/student/tianjiu/Documents/ta/cs160/proj0/calc2/"
		self.configPath="/cs/class/cs160/grading/AutoGrading/"
		self.workDir="/cs/class/cs160/grading/AutoGrading/source/"
		self.students={}
		self.test=Test()
		test=""
		self.testPath=self.configPath+"testcase."+str(proj)+".xml"
		self.testInput=self.configPath+"testinput."+str(proj)+".xml"
		self.testDir=self.configPath+"testcases"+str(proj)+"/"
		self.penalty={}
		self.emailed={}
		self.proj=proj
		self.folder=folder
		self.command=command


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
			print filePath
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
				stu.testResult.result[ID]=CaseResult(ID,float(items[i]),items[i+1],0)
			stu.graded=True
	
	def loadPenaltyList(self,filePath):
		print "Loading penalty list"
		try:
			fp=open(filePath,"rb+")
		except:
			print "Penalty List doesn't exist!"
			return 
		self.penalty={}	
		for line in fp.readlines():
			items=line.split(',')
			if(len(items)<2):
				continue
			self.penalty[items[0]]=int(items[1])
	
	def loadEmailedList(self,filePath):
		print "Loading emailed list"
		try:
			fp=open(filePath,"rb+")
		except:
			print "Emailed List doesn't exist!"
			return 
		self.emailed={}	
		for line in fp.readlines():
			self.emailed[line.strip()]=True

		try:
			fp=open(filePath,"rb+")
		except:
			print "Emailed List doesn't exist!"
			return
 
	def addEmailedList(self,name,filePath):
		try:
			fp=open(filePath,"ab+")
			fp.write(name+"\n")
			fp.close()
		except:
			print "Emailed List doesn't exist!"
		
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
			output.write("\t\t<Input>"+Tin.replace("<","&lt;") +"</Input>\n")
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
		print "work dir: "+os.getcwd()
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
		Shell.call(["rm","-r",self.workDir+"proj"+str(self.proj)+"/extract"])
		Shell.call(["mkdir",self.workDir+"proj"+str(self.proj)+"/extract"])
		os.chdir(self.workDir+"proj"+str(self.proj))
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
			if(not os.path.isdir("extract/"+alias+"/"+self.folder)):
				Shell.call(["mkdir","extract/"+alias+"/"+self.folder])
				Shell.call(["tar","-zxvf",fileName,"-C","extract/"+alias+"/"+self.folder])
			
	def testGrade(self,path):
		#self.loadPenaltyList(self.configPath+"penalty.list")
		
		Shell.call(["clear"])
		print "Testing grading for project "+str(self.proj)
		self.genTestCase()
		#path="/cs/class/cs160/project0/solution/"
		#path="/cs/class/cs160/grading/AutoGrading/source/proj0/extract/htl/calc"
		#path=(self.workDir+str(self.proj)+"/extract/"+alias+"/"+self.folder+"/")
		os.chdir(path)
		print "Working directory: "+os.getcwd()
		result=self.test.runTest(self.command,self.log,self.error)
		#stu.checkPenalty(self.penalty)
		for key in sorted(result.result.keys()):		 
			print key+ "\t"+str(result.result[key].result)
			if(result.result[key].result!=0):
				print "\t"+result.result[key].error
		print result.score
		
		#s=Student("Keith")
		#s.testResult=result
		#s.save("Report.htm")
	
	def testGrade2(self,alias):
		self.loadPenaltyList(self.configPath+"penalty.list")
		
		Shell.call(["clear"])
		print "Testing grading for project "+str(self.proj)
		self.genTestCase()
		#path="/cs/class/cs160/project0/solution/"
		#path="/cs/class/cs160/grading/AutoGrading/source/proj0/extract/htl/calc"
		path=(self.workDir+"proj"+str(self.proj)+"/extract/"+alias+"/"+self.folder+"/")
		os.chdir(path)
		print "Working directory: "+os.getcwd()
		#result=self.test.runTest(self.command,self.log,self.error)
		stu=Student(alias,"","","")
		stu.test=self.test
		stu.grade(self.command,self.log,self.error)
		result=stu.testResult
		for key in sorted(result.result.keys()):		 
			print key+ "\t"+str(result.result[key].result)
			if(result.result[key].result!=0):
				print "\t"+result.result[key].error
		print "Initial Score: " + str(result.score)
		stu.checkPenalty(self.penalty)
		print "After Score: "+ str(result.score)
		
		#s=Student("Keith")
		#s.testResult=result
		#s.save("Report.htm")

	def emailResult(self):
		Shell.call("clear")
	
                self.genTestCase()
	
		self.loadStudentList(self.configPath+"studentFullList")
		self.loadStudentResult(self.configPath+"proj"+str(self.proj)+".result")
		self.loadPenaltyList(self.configPath+"penalty.list")
		self.loadEmailedList(self.configPath+"emailed.list")
		
		average=81.1
		median=87.5
		stdev=18.8	

		count=0
		for alias in self.students.keys():
			count=count+1
			if(alias in self.emailed): 
				continue

			Shell.call("clear")
			stu=self.students[alias]
			stu.test=self.test
			print "Emailing: "+alias+", "+str(count)
			#stu.sendResult(To="kpavery@cs.ucsb.edu",proj=self.proj,penalty=self.penalty,average=average,median=median,stdev=stdev)
			#stu.sendResult(To="sherwood@cs.ucsb.edu",proj=self.proj,penalty=self.penalty,average=average,median=median,stdev=stdev)
			#stu.sendResult(To="tianjiu@umail.ucsb.edu",proj=self.proj,penalty=self.penalty,average=average,median=median,stdev=stdev)
			print str(len(self.emailed)+count)
			stu.sendResult(proj=self.proj,penalty=self.penalty,average=average,median=median,stdev=stdev)
			self.addEmailedList(stu.name,self.configPath+"emailed.list")

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
		self.loadStudentResult(self.configPath+"proj"+str(self.proj)+".result")
		self.loadPenaltyList(self.configPath+"penalty.list")
		self.loadEmailedList(self.configPath+"emailed.list")
		
		average=70.1
		median=71.1
		stdev=25.9	

		count=0
		for alias in self.students.keys():
			Shell.call("clear")
			stu=self.students[alias]
			print "Grading: "+alias
			print stu.graded
			stu.test=self.test
			if(stu.graded):
				continue
			os.chdir(self.workDir+"proj"+str(self.proj)+"/extract/"+alias+"/"+self.folder+"/")
			print "Working directory: "+os.getcwd()
			#self.test.runTest(self.command,self.log,self.error)
			stu.test=self.test
			stu.grade(self.command,self.log,self.error)
			stu.checkPenalty(self.penalty)
			self.genStudentResult(self.configPath+"proj"+str(self.proj)+".result")	
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

	while(False):
		Shell.call("clear")
		Name = str(input("Enter the student's name: ")).strip()
		print "grading:"+Name
		
		proj1.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj1/extract/"+Name+"/parse")
		name=input("")
	if options.grade:
		print "grading"
	else:
		print "No Options!"





#proj0=Grading()
#proj0.genTestOutput(["./calc"],"/cs/class/cs160/project0/solution/")
#proj0.genTestCase()
#proj0.getStudentName()
#proj0.loadStudentList(proj0.configPath+"studentFullList")
#proj0.genStudentSubmission()
#proj0.genStudentResult(proj0.answerPath+"proj0.result")
#proj0.startGrading()
#proj0.genStudentResult("proj0.result.xls")
#proj0.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj0/extract/stirling/calc")


proj1=Grading(1,"parse",["./lang"])
#proj1.genTestOutput(["./lang"],"/cs/class/cs160/project1/solution/")
#proj1.genTestCase()
#proj1.testGrade("/cs/class/cs160/project1/solution/")
#proj1.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj1/parse")
#proj1.testGrade("/cs/student/tianjiu/Downloads/parse")
#proj1.loadStudentList(proj1.configPath+"studentFullList")
#proj1.genStudentSubmission()
#proj1.startGrading()
#proj1.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj1/extract/klee00/parse")
#proj1.testGrade("/cs/class/cs160/grading/AutoGrading/test/cooney")
#proj1.testGrade2("ktbui")


proj2=Grading(2,"ast",["./lang"])
#proj2.genTestOutput(["./lang"],"/cs/class/cs160/project2/solution/")
#proj2.genTestCase()
#proj2.testGrade("/cs/class/cs160/project1/solution/")
#proj2.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj1/parse")
#proj2.testGrade("/cs/student/tianjiu/Downloads/parse")
#proj2.testGrade2("jstaahl")
proj2.testGrade2("briankwan")
#proj2.startGrading()
#proj2.emailResult()

proj3=Grading(3,"typecheck",["./lang"])
#proj3.genTestOutput(["./lang"],"/cs/class/cs160/project2/solution/")
#proj3.genTestCase()
#proj3.testGrade("/cs/class/cs160/project1/solution/")
#proj3.testGrade("/cs/class/cs160/grading/AutoGrading/source/proj1/parse")
#proj3.testGrade("/cs/student/tianjiu/Downloads/parse")
#proj3.loadStudentList(proj3.configPath+"studentFullList")
#proj3.genStudentSubmission()
#proj3.testGrade2("ktbui")


if __name__ == '__main__':
    sys.exit(main())
