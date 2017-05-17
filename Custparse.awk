{
#Customer Parsing
	if (match ($1, "cust_name:")) {
		i=2;
		while (i<=NF) {
				customername = customername" "$i; 
			i++
		}
	}
	else if (match ($1, "cust_address:")) {
		i=2;
		while (i<=NF) {
				address = address" "$i; 
			i++
		}
	}
	else if (match ($1, "cust_phone:")) 
	{
		phone = $2
	}
	else if (match ($1, "cust_email:")) 
	{
		email = $2
	}
	if (length (customername)) {
		if (length (address)){
			if (length (phone)){
				if (length (email)){
					customername = substr (customername, 2)
					address = substr (address, 2)
						print "INSERT INTO customer"
						print "     VALUES ('"email"', '"customername"', '"address"', '"phone"');"
						print ""
						print ""
						print "INSERT INTO shopping_basket (email)"
						print "     VALUES ('"email"');"
						print ""
						customername = "";
						address = "";
						phone = "";
						email = "";
				}
			}
		}
	}
}

