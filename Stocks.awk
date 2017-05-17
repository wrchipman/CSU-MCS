{
#Stocks Parsing


	if (match ($1, "isbn:")) {
		isbn =$2
	}

	if (length (isbn)) {
		print "stmt.executeUpdate ("
		print "\"INSERT INTO stocks"
		print "     VALUES ('"isbn"', 'ATL', '2')\" );"
		print ""
		print "stmt.executeUpdate ("
		print "\"INSERT INTO stocks"
		print "     VALUES ('"isbn"', 'SFR', '2')\" );"
		print ""
		print "stmt.executeUpdate ("
		print "\"INSERT INTO stocks"
		print "     VALUES ('"isbn"', 'CHI', '2')\" );"
		print ""
		isbn = ""
	}	





}
