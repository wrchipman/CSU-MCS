{
#Publisher Parsing

	if (match ($1, "publisher:")) {
		i=2;
		while (i<=NF) {
				publisher = publisher" "$i; 
			i++
		}
	} 
	else if (match ($1, "pub_address:")) 
	{
		i=2;
		while (i<=NF) {
				pub_address = pub_address" "$i; 
			i++
		}
	}
	else if (match ($1, "pub_url:")) 
	{
		i=2;
		while (i<=NF) {
				pub_url = pub_url" "$i; 
			i++
		}
	}
	else if (match ($1, "pub_phone:")) 
	{
		i=2;
		while (i<=NF) {
				pub_phone = pub_phone" "$i; 
			i++
		}
	}








	if (length (publisher)) {
		if (length (pub_address)){
			if (length (pub_url)){
				if (length (pub_phone)){
					publisher = substr (publisher, 2)
					pub_address = substr (pub_address, 2)
					pub_url = substr (pub_url, 2)
					pub_phone = substr (pub_phone, 2)
					print "INSERT INTO publisher"
					print "     VALUES ('"publisher"', '"pub_address"', '"pub_url"', '"pub_phone"');"
					print ""
					publisher = "";
					pub_address = "";
					pub_url = "";
					pub_phone = "";
				}
			}	
		}
	}
}
