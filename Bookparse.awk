{
#Book Parsing


	if (match ($1, "isbn:")) {
		isbn =$2
	} 
	else if (match ($1, "author:")) {
		i=2;
		while (i<=NF) {
			if (!match ($i, "&")) {
				authorname = authorname" "$i; 
			}		 
			else { 
				while (i<=NF) {
					if (!match ($i, "&")) {
						authorname2 = authorname2" "$i;
					} 
					else  {
					}

					i++
				}
			}
			i++
		}	
	}
	else if (match ($1, "title:")) 
	{
		i=2;
		while (i<=NF) {
				title = title" "$i; 
			i++
		}
	}
	else if (match ($1, "date:")) 
	{
				date = $2;
	}
	else if (match ($1, "price:")) 
	{
		price = $2	
	}
	else if (match ($1, "book_url:")) 
	{
		url = $2
	}
	else if (match ($1, "publisher:")) 
	{
		i=2;
		while (i<=NF) {
				publisher = publisher" "$i; 
			i++
		}
	}


	if (length (isbn)) {
		if (length (title)){
			if (length (authorname)){
				if (length (date)){
					if (length (price)){
						if (length (url)){
							if (length (publisher)){
								authorname = substr (authorname, 2)
								title = substr (title, 2)
								publisher = substr (publisher, 2)
								print "stmt.executeUpdate ("
								print "\"INSERT INTO book"
								print "     VALUES ('"isbn"', '"price"', '"title"', '"date"', '"url"', '"publisher"')\" );"
								print ""
								print "stmt.executeUpdate ("
								print "\"INSERT INTO written_by"
								print "     VALUES ('"isbn"', '"authorname"')\" );"
								print ""
								authorname = "";
								if (length (authorname2)) {
			 						authorname2 = substr (authorname2, 2)
									print "stmt.executeUpdate ("
									print "\"INSERT INTO written_by"
									print "     VALUES ('"isbn"', '"authorname2"')\" );"
									print ""
									authorname2 = "";
								}
								title = ""
								publisher = ""	
							}
						}
					}
				}
			}	
		}
	}	





}
