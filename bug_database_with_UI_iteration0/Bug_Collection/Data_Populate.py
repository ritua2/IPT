import mysql.connector
import argparse
import subprocess



PIPE = subprocess.PIPE

def getfullpath(model):
	r = subprocess.run("pwd",shell= True,stdout=PIPE)
	path=r.stdout.decode().strip()
	return "{}/{}".format(path,model)

def getbuglist(fullpath):
	r = subprocess.run("ls -1 {}".format(fullpath),shell = True, stdout = PIPE)
	lines=r.stdout.decode().strip()
	lines=lines.split("\n")
	rl = []
	for l in lines:
		rl.append((l,"{}/{}".format(fullpath,l)))
	return rl 

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
	mycursor.execute("CREATE TABLE bugCollection (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(255), model VARCHAR(255), folderPath VARCHAR(255))")

	for model in ["MPI","OPENMP", "CUDA"]:
		fullpath=getfullpath(model)
		print(fullpath)
		buglist=getbuglist(fullpath)
		for bug,path in buglist:
			print(bug,path)
			sql = "INSERT INTO bugCollection (name, model,folderPath) VALUES (%s, %s, %s)"
			value=("{}_{}".format(model,bug),model,path)
			mycursor.execute(sql, value)
			mydb.commit()


