import mysql.connector
import argparse
import subprocess
import json


PIPE = subprocess.PIPE

def getfullpath(model):
	r = subprocess.run("cd",shell= True,stdout=PIPE)
	#print(r)
	path=r.stdout.decode().strip()
	return "{}\\{}".format(path,model)

def getbuglist(path):
	r = subprocess.run("dir {}".format(path),shell = True, stdout = PIPE)
	lines=r.stdout.decode().strip()
	#if "Bug" in lines:
	#	print(lines)
	lines=lines.split("\n")
	rl = []
	for l in lines:
		if "Bug" in l and "DIR" in l:
			#print(l)
			first, *middle, last = l.split()
			#print(last)
		#if l.isspace() and len(l) != 0:
			#first, *middle, last = l.split()
			#print(last)
			#rl.append((l,"{}{}".format(path,l)))
			rl.append((l,"{}\\{}".format(path,last)))
	return rl 

def get_tags_list(path):
	#print (path)
	filename="{}\\tag.json".format(path)
	data = {'Tags':[]}
	with open(filename) as f:
		lines = f.read().strip()
		if lines != "":
			data=json.loads(lines)

	return data['Tags']

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('-host', required = True)
	parser.add_argument('-user',required = True)
	parser.add_argument('-passwd',required=False)
	parser.add_argument('-database',required=True)
	args = parser.parse_args()
	password = args.passwd if args.passwd != None else ""
	mydb = mysql.connector.connect( host=args.host, user=args.user, passwd=password, database=args.database)
	mycursor = mydb.cursor()

	# creating the tables
	mycursor.execute("DROP TABLE IF EXISTS bugRecord")
	mycursor.execute("DROP TABLE IF EXISTS bugCollection")
	mycursor.execute("DROP TABLE IF EXISTS bugCatagories")
	mycursor.execute("DROP TABLE IF EXISTS programmingModel")


	mycursor.execute("CREATE TABLE bugCollection (ID INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255) UNIQUE, folderPath VARCHAR(255) ) ")
	mycursor.execute("CREATE TABLE bugCatagories (ID INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255) UNIQUE) ")
	mycursor.execute("CREATE TABLE programmingModel(ID INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255)UNIQUE ) ")
	
	mycursor.execute("CREATE TABLE bugRecord (ID  INT AUTO_INCREMENT PRIMARY KEY, Bug_id INTEGER, PM_id INTEGER, Cate_id INTEGER,\
		FOREIGN KEY(Bug_id) REFERENCES bugCollection(ID),\
		FOREIGN KEY(PM_id) REFERENCES programmingModel(ID),\
		FOREIGN KEY(Cate_id ) REFERENCES bugCatagories(ID))")
	mydb.commit()
	# insert into tables

	# insert data into programming model table
	models =  ["MPI","OPENMP", "CUDA"]
	for model in models:
		sql = "INSERT INTO programmingModel (name) VALUES ('{}')".format(model)
		mycursor.execute(sql)
	
	mydb.commit()
	

	# insert data into other tables
	for model in models:
		fullpath=getfullpath(model)
		#print(fullpath)
		buglist=getbuglist(fullpath)
		for bug,path in buglist:
			#print(bug,path)

			#insert into bug collection table
			sql = "INSERT INTO bugCollection (name, folderPath) VALUES (%s, %s)"
			value=("{}_{}".format(model,bug),path)
			mycursor.execute(sql, value)
			mydb.commit()
			bug_id = mycursor.lastrowid

			#insert into bug categories table
			tags = get_tags_list(path)
			for t in tags:
				sql = "INSERT IGNORE INTO bugCatagories (name) VALUES ('{}')".format(t)
				mycursor.execute(sql)
				mydb.commit()

			#insert into bug record table
			get_pm_id_query = "select ID from programmingModel where name = '{}'".format(model)
			mycursor.execute(get_pm_id_query)
			pm_id = mycursor.fetchone()[0]

			for t in tags:
				get_tag_id_query = "select ID from bugCatagories where name = '{}'".format(t)
				mycursor.execute(get_tag_id_query)
				tag_id = mycursor.fetchone()[0]
				sql = "INSERT INTO bugRecord (Bug_id,PM_id, Cate_id) VALUES ({},{},{})".format(bug_id,pm_id,tag_id)
				#value = (bug_id,pm_id,tag_id)
				mycursor.execute(sql)
			mydb.commit()
