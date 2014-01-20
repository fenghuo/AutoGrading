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
	def __init__(self,ID,testIn,testResult,score,level):
		self.testID=ID
		self.testIn=testIn
		self.testResult=testResult
		self.score=score
		self.level=level
		self.errorMsg=""
		
	def load():
		print "loading test case: "+testID	
		
	def run(self,command,log,error):
		self.errorMsg=""
		testID=self.testID
		tin=open(self.testIn,"r")
		tout=open(testID+".output","wr")
		tcode=open(testID+".c","wr")
		result=subprocess.call(command,stdin=tin,stdout=tout,stderr=tcode)
		tin.close()
		tout.close()
		tcode.close()
		if(result==0):
			if(self.level=='1'):
				return self.testScan(log,error)
			elif(self.level=='2'):
				return  self.testParse(log,error)
			else:
				return self.testCode(log,error)
		else:
			self.errorMsg="compiler runtime error\n"

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
			return 0
		else:
			self.errorMsg="Scanner Error\n"
			return 1


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

class Test:
	def __init__(self):
		self.testCases=[]
		self.result=[]
		self.errors=[]
	
	def loadTest():
		print "loading tests:"
	
	def addTest(self,ID,testIn,testResult,score,level):
		self.testCases.append(TestCase(ID,testIn,testResult,score,level))	
	
	def runTest(self,command,log,error):
		print "running test:"	
		for case in self.testCases:
			self.result.append(case.run(command,log,error))
			self.errors.append(case.errorMsg)
		
	def getScore(self):
		score=0
		for i in range(0,len(self.testCases)):
			if(self.result[i]==0):
				score+=int(self.testCases[i].score)
		return score
		
			
class Student:
	def __init__(self,alias):
		self.name=""
		self.score=0
		self.testResult=[]

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

		self.test.runTest(["./calc"],self.log,self.error)
		print '\n'.join(str(i) for i in self.test.result)
		print self.test.getScore()
		
		for student in self.students:
			student.grade(["./calc"],self.log,self.error)
			print student.name+" "+student.score


proj0=Grading()
proj0.genTestCase()
proj0.startGrading()
