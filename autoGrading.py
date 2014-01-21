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
import csv
import xml.etree.ElementTree as ET
from os.path import isdir, join
from optparse import OptionGroup, OptionParser
from subprocess import Popen, PIPE, STDOUT,call

class TestCase:
	def __init__(self,ID,testIn,testResult,score):
		self.testID=ID
		self.testIn=testIn
		self.testResult=testResult
		self.score=score
		self.errorMsg=""
		self.result=CaseResult(ID,1,"",score)
		
	def load():
		print "loading test case: "+testID	
		
	def run(self,command,log,error):
		self.errorMsg=""
		testID=self.testID
		tin=open(self.testIn,"r")
		tout=open(testID+".output","wr")
		tcode=open(testID+".c","wr")
		subprocess.call(command,stdin=tin,stdout=tout,stderr=tcode)
		tin.close()
		tout.close()
		tcode.close()

	def testScan(self,log,error):
		tout=open(self.testID+".output","r").readlines()
		tresult=open(self.testResult,"r").readlines()
		lineCounter=None
		lineExpect=''.join(tresult)
		for line in reversed(tout):
			if(line.find("scan error")>=0):
				lineCounter=line[line.find('line')+4:]				
				break
		if(lineCounter==lineExpect):
			self.result.result=0
		else:
			self.result.errorMsg="Scanner Error! Expect Error Line: "+lineExpect+" ,your error line: "+lineCounter+"\n"
			self.result.result=1;
		return self.result


	def testParse(self,log,error):
                tout=open(self.testID+".output","r")
		result=[]
		for line in tout:
			if(line.find('fillcolor=powderblue')>=0):
				for c in line:
					if(c>='0' and c<='9'):
						break;
					elif(c=='"' or c=='\t' or c==' '):
						continue
					else:
						result.append(c)
			
		tresult=open(self.testResult,"r").readlines()
		result=''.join(result).strip()	
		tresult=''.join(tresult).strip()
		if(result==tresult):
			return 0
		else:
			self.errorMsg="Parse Tree Error\n"	
			return 1
				
	def testCode(self,log,error):
		testID=self.testID
		tcoutput=open(testID+".c.output","wr")
		result=0
		result=subprocess.call(["gcc","-lm",testID+'.c',"-o",testID],stdout=log,stderr=error)
		if(result==0):
			result=subprocess.call(["./"+testID],stdout=tcoutput,stderr=error)
			if(result==0):
				result=subprocess.call(["diff","-bBi",self.testResult,testID+".c.output"],stdout=log,stderr=error)
				if(result!=0):
					self.errorMsg="C code execution output error\n"
			else:
				self.errorMsg="C code runtime error\n"
		else:
			self.errorMsg="C code Compile error\n"
				
		tcoutput.close()
		return result

class TestP0Scan(TestCase):
	def run(self,command,log,error):

		TestCase.run(self,command,log,error)

		tout=open(self.testID+".output","r").readlines()
		tresult=open(self.testResult,"r").readlines()
		lineCounter=""
		lineExpect=''.join(tresult)
		
		for line in reversed(tout):
			if(line.find("scan error")>=0):
				lineCounter=line[line.find('line')+4:]				
				break
		if(lineCounter==lineExpect):
			self.result.result=0
		else:
			self.result.errorMsg="Scanner Error! Expect Error Line: "+str(lineExpect)+" ,your error line: "+str(lineCounter)+"\n"
			self.result.result=1
		return self.result

class TestP0Parse(TestCase):
	def run(self,command,log,error):
                tout=open(self.testID+".output","r")
		result=[]
		for line in tout:
			if(line.find('fillcolor=powderblue')>=0):
				for c in line:
					if(c>='0' and c<='9'):
						break;
					elif(c=='"' or c=='\t' or c==' '):
						continue
					else:
						result.append(c)
			
		tresult=open(self.testResult,"r").readlines()
		result=''.join(result).strip()	
		tresult=''.join(tresult).strip()
		if(result==tresult):
			self.result.result=0
		else:
			self.result.errorMsg="Parse Tree Error\n"	
			self.result.result=1
		return self.result

class TestP0Code(TestCase):
	def run(self,command,log,error):

		testID=self.testID
		tcoutput=open(testID+".c.output","wr")
		result=0
		result=subprocess.call(["gcc","-lm",testID+'.c',"-o",testID],stdout=log,stderr=error)
		if(result==0):
			result=subprocess.call(["./"+testID],stdout=tcoutput,stderr=error)
			if(result==0):
				result=subprocess.call(["diff","-bBi",self.testResult,testID+".c.output"],stdout=log,stderr=error)
				if(result!=0):
					self.result.errorMsg="C code execution output error\n"
			else:
				self.result.errorMsg="C code runtime error\n"
		else:
			self.result.errorMsg="C code Compile error\n"
				
		tcoutput.close()
		self.result.result=result
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
		self.result=[]

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

	def runTest(self,command,log,error):
		self.result=None
		print "running test:"	
		for case in self.testCases:
			self.result.result.append(case.run(command,log,error))
		self.result.score=self.getScore()
		return self.result
		
	def getScore(self):
		score=0
		for i in range(0,len(self.result.result)):
			if(self.result.result[i].result==0):
				score+=int(self.result.result[i].score)
		return score
		
			
class Student:
	def __init__(self,alias):
		self.name=""
		self.testResult=None

	def grade(self,test):
		print "grading:"
		self.result=test.runTest(command,log,error)

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
		fp = open(textfile, 'ab')	
		fp.write("<name>"+self.name+"</name>\n")
		fp.write("<result>\n")
		fp.write("<total>"+str(self.testResult.score)+"</total>\n")	
		fp.write("<TestCase>\n")
		for r in self.testResult:
			fp.write("<ID>"+str(r.ID)+"</ID>\n")
			fp.write("<result>"+str(r.result)+"</result>\n")
			fp.write("<error>"+str(r.error)+"</error>\n")
		fp.write("<TestCase>\n")		
		fp.write("</result>\n")
		
class Grading:
	def __init__(self):
		self.workDir="/cs/student/tianjiu/Documents/ta/cs160/proj0/calc2"
		self.students=[]
		self.test=Test()
		test=""
		self.testPath="/cs/student/tianjiu/Documents/ta/cs160/AutoGrading/testcase.xml"
		self.testDir="/cs/student/tianjiu/Documents/ta/cs160/AutoGrading/testcases/"
		
	def __del__(self):
		self.log.close()
		self.error.close()

	def runShell(self,command):
		self.log.write(time.strftime('%X %x %Z')+'\n')
		self.log.write("command: "+ ' '.join(command)+'\n')
		self.error.write(time.strftime('%X %x %Z')+'\n')
		self.error.write("command: "+ ' '.join(command)+'\n')
		self.log.flush()
		self.error.flush()
		return subprocess.call(command,stdout=self.log,stderr=self.error)
	
	def genTestCase(self):
		print "generating all test cases:\n"
		subprocess.call(["rm","-rf",self.testDir])
		subprocess.call(["mkdir",self.testDir])		
		tree = ET.parse(self.testPath)
		root = tree.getroot()
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

	def startGrading(self):
		subprocess.call("clear")
		print "start grading"
		os.chdir(self.workDir);	
		print "working directory: \n"+os.getcwd()
		self.log = open("log","w")
		self.error = open("error","w")
		self.runShell(["make","clean"])
		if(self.runShell(["make"])!=0):
			print "compile error"
			return
	
		#case=TestCase("1","3.in","1.result","1.c.result",3)
		#print case.run(["./calc"],self.log,self.error)
		#print case.errorMsg
		#print case.testScan("","")

		result=self.test.runTest(["./calc"],self.log,self.error)
		print '\n'.join(str(i.result) for i in result.result)
		print result.score
		
		for student in self.students:
			student.grade(["./calc"],self.log,self.error)
			print student.name+" "+student.score

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
proj0.genTestCase()
proj0.startGrading()



if __name__ == '__main__':
    sys.exit(main())
