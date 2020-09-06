from django.shortcuts import render
from django.http import HttpResponse

import os,sys
import cx_Oracle

# oracle database connection
conn = cx_Oracle.connect('scott', 'tiger','localhost:1521/orcl')
cur = conn.cursor()

# convert str to html string
# tag = html tag && cls = html class
def makeHtml(str,tag,cls):
	if cls!=0:
		return '<'+tag+ 'class="'+ cls +'">'+str+'</'+tag+'>'
	else:
		return '<'+tag+'>'+str+'</'+tag+'>'

# V1: output format is table 
# porb : problem definition
# sql : sql statement
def executeSQL_V1(prob,sql):
	cur.execute(sql)
	rows = cur.fetchall()
	length = len(cur.description)
	count = 0

	result = '<div class="jumbotron">'
	result = result + makeHtml("Q: " + prob, 'p','h2') 	
	result = result + makeHtml("SQL: " + sql,'p','h3')+  '<hr class="my-4">'

	content = '<table class="table table-striped"><thead><tr><th scope="col">#</th>'
	for i in range(0,length):
		content = content + '<th scope="col">' + str(cur.description[i][0]) + '</th>'
	content = content + '</tr><thead><tbody>'
	

	if len(rows) == 0:
		result = "RESULT NOT FOUND\n"
	else:
		for r in rows:
			content = content + '<tr>'
			content = content + '<td>'  + str(count) + '</td>' 
			count = count + 1
			for idx in range(length):
				content = content + '<td>' + str(r[idx]) + '</td>'
			content = content + '</tr>'
		content = content + '</tbody></table>'

	result = result + content + '</div>'
	return result;

# V2: the number of output is signle value
def executeSQL_V2(prob, sql):
	cur.execute(sql)
	rows = cur.fetchall()
	
	result = '<div class="jumbotron">'
	result = result + makeHtml("Q: " + prob, 'p','h2') 	
	result = result + makeHtml("SQL: " + sql,'p','h3')+  '<hr class="my-4">'

	if len(rows) == 0:
		result = "RESULT NOT FOUND\n"
	else:
		for r in rows:
			result = result + makeHtml(str(r[0]),'p','h5') + "<br/>"

	result = result + '</div>'
	return result;


def main(request):
	html_head = """<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <title>Hello, world!</title>
</head>
<body>"""

	html_foot = """<script src="https://code.jquery.com/jquery-3.2.1.slim.min.js" integrity="sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" integrity="sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q" crossorigin="anonymous"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" integrity="sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl" crossorigin="anonymous"></script>
  </body>
</html>"""

## MODIFY HERE ! ##
#########################################################################################################################
	
	# EX1
	prob1 = "print  information of users who go to specific destination."
	sql1 = "SELECT D.DESTINATIONNAME, U.* FROM USERINFO U, LINE L, RESERVATION R, DESTINATION D WHERE U.USERID=R.USERID AND L.LINEID=R.LINEID AND L.DESTINATIONID = D.DESTINATIONID"
	result1 = executeSQL_V1(prob1, sql1);

	#EX2
	prob2 = "sort by distance which is most far of each destination."
	sql2 = "SELECT D.DESTINATIONNAME AS STARTPOINT, MAX(DS.DISTANCE) AS DISTANCEMAX FROM DESTINATION D, DISTANCE DS WHERE D.DESTINATIONID = DS.DESTINATIONID1 GROUP BY D.DESTINATIONNAME ORDER BY DISTANCEMAX"
	result2 = executeSQL_V1(prob2, sql2);

	#EX3
	prob3 = "print average age of each line's users."
	sql3 ="SELECT L.LINEID, AVG(U.AGE) FROM LINE L, USERINFO U, RESERVATION R WHERE L.LINEID=R.LINEID AND U.USERID=R.USERID GROUP BY L.LINEID"
	result3 = executeSQL_V1(prob3, sql3);

	#EX4
	prob4 = "print line's start time which run by 'saemaul'."
	sql4 ="SELECT L.LINEID, L.STARTTIME FROM LINE L WHERE L.TRAINID IN(SELECT T. TRAINID FROM TRAIN T, TRAINTYPE P WHERE T.TRAINNAME=P.TRAINTYPE)"
	result4 = executeSQL_V1(prob4, sql4);
	
	#EX5
	prob5 = "remove duplicate tuples in reservation table."
	sql5 ="SELECT DISTINCT * FROM RESERVATION"
	result5 = executeSQL_V1(prob5, sql5);

	#EX6_1
	prob6_1 = "print start time of each destinations."
	sql6_1 ="SELECT L.LINEID, D2.DESTINATIONNAME, D1.DESTINATIONNAME, L.STARTTIME, RANK() OVER(PARTITION BY L.DESTINATIONID ORDER BY L.STARTTIME) FROM LINE L, DESTINATION D1, DESTINATION D2 WHERE L.DESTINATIONID = D1.DESTINATIONID AND L.STARTID=D2.DESTINATIONID ORDER BY L.DESTINATIONID"
	result6_1 = executeSQL_V1(prob6_1, sql6_1);

	#EX6_2
	prob6_2 = "print amount of money of each user."
	sql6_2 = "SELECT U.USERNAME AS USERNAME, SUM(TP.PRICE + DP.PRICE + SP.PRICE)/2 AS TICKETPRICE FROM USERINFO U, RESERVATION R, TRAIN T, TRAINTYPE TT, DESTINATION D1, DESTINATION D2, DISTANCE DS, SEAT S,  DESTINATIONPRICE DP, TRAINPRICE TP, SEATPRICE SP, LINE L WHERE R.LINEID = L.LINEID AND L.STARTID = D1.DESTINATIONID AND L.DESTINATIONID = D2.DESTINATIONID AND D1.DESTINATIONID = DS.DESTINATIONID1 AND D2.DESTINATIONID = DS.DESTINATIONID2 AND DS.DISTANCE = DP.DISTANCE AND L.TRAINID = T.TRAINID AND T.TRAINTYPEID = TP.TRAINTYPEID AND R.SEATID = S.SEATID AND S.SEATTYPEID = SP.SEATTYPEID AND U.USERID = R.USERID GROUP BY U.USERNAME"
	result6_2 = executeSQL_V1(prob6_2, sql6_2);

	html_body = result1 + result2 + result3 + result4 + result5 + result6_1 + result6_2;


##########################################################################################################################

	# DO NOT MODIFY
	output = html_head + html_body + html_foot
	return HttpResponse(output)
