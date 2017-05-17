{
	if (match ($1, "author:")) {
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
	else if (match ($1, "author_url:")) 
	{
		url = $2
	}
	if (length (authorname)) {
		if (length (url)){
			authorname = substr (authorname, 2)
			print "INSERT INTO author"
			print "     VALUES ('"authorname"', '"url"');"
			print ""
			authorname = "";
			if (length (authorname2)) {
			 		authorname2 = substr (authorname2, 2)
					print "INSERT INTO author"
					print "     VALUES ('"authorname2"', '"url"');"
					print ""
	
					authorname2 = "";
			}
		}
	}
}
	

