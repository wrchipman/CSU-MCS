/*
*****************************************************************
*								*
*		William Chipman					*
*		CS430						*
*		21 July 2009					*	
*		Lab4A.java					*
*								*
*****************************************************************/	









import java.sql.*;
import java.io.File;
import java.text.*;
import java.util.Locale;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class Lab4A {
  public static void main(String args[]){

    Connection con = null;
    NumberFormat nf = NumberFormat.getCurrencyInstance(Locale.US);

    try {
      Statement stmt, stmt2;
      ResultSet rs, rs2;

      // Register the JDBC driver for MySQL.
      Class.forName("com.mysql.jdbc.Driver");

      // Define URL of database server for
      // database named 'user' on the faure.
      String url =
            "jdbc:mysql://faure/chipman";

      // Get a connection to the database for a
      // user named 'user' with the password
      // 123456789.
      con = DriverManager.getConnection(
                        url,"chipman", "temppasswd");

      // Display URL and connection information
      System.out.println("URL: " + url);
      System.out.println("Connection: " + con);

      // Get a Statement object
      stmt = con.createStatement();
      stmt2 = con.createStatement();
      // As a precaution, delete any existing relations if
      // they already exist as residue from a previous run. 

      // Drop book
      try{
        stmt.executeUpdate("DROP TABLE book");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No book table to delete");
      }//end catch

      // Drop author
      try{
        stmt.executeUpdate("DROP TABLE author");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No author table to delete");
      }//end catch

      // Drop publisher
      try{
        stmt.executeUpdate("DROP TABLE publisher");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No publisher table to delete");
      }//end catch

      // Drop warehouse
      try{
        stmt.executeUpdate("DROP TABLE warehouse");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No warehouse table to delete");
      }//end catch

      // Drop customer
      try{
        stmt.executeUpdate("DROP TABLE customer");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No customer table to delete");
      }//end catch

      // Drop shopping_basket
      try{
        stmt.executeUpdate("DROP TABLE shopping_basket");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No shopping_basket table to delete");
      }//end catch

      // Drop stocks
      try{
        stmt.executeUpdate("DROP TABLE stocks");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No stocks table to delete");
      }//end catch


      // Drop written_by
      try{
        stmt.executeUpdate("DROP TABLE written_by");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No written_by table to delete");
      }//end catch
     




    // Create Table author

	stmt.executeUpdate (
		"CREATE TABLE author (name varchar (255) PRIMARY KEY, URL varchar (255))" );
    

    // Insert values into author

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Andrews, Gregory F.', 'http://www.cs.arizona.edu/people/greg/') ");

     
	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Babb, Robert G., II, ed.', 'http://www.cs.arizona.edu/people/greg/') ");

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Carriero, Nicholas', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Gelernter, David', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Gehani, Naraim', 'http://www.cs.yale.edu/people/faculty/carriero.html')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('McGettrick, Andrew D., eds.', 'http://www.cs.yale.edu/people/faculty/carriero.html')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Hansen, Per Brinch', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Hatcher, Philip J.', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Hoare, C. A. R.', 'http://www.cs.unh.edu/~pjh/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Hockney, R. W.', 'http://research.microsoft.com/users/thoare/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Jesshope, C. R.', 'http://research.microsoft.com/users/thoare/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Lastovetsky, Alexey L.', 'http://perun.hscs.wmin.ac.uk/roger/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Lester, Bruce P.', 'http://www.cs.ucd.ie/staff/Alexeyl/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Leopold, Claudia', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Lewis, Ted G.', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author  VALUES ('Lipovski, G. J.', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Malek, M.', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Perrott, R. H.', 'http://www.ece.utexas.edu/ece/people/profs/Lipovski.html')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Polychronopoulos, Constantine D.', 'none')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Raynal, Michel', 'http://www.ece.uiuc.edu/fachtml/polychronopoulos.html')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Schiper, Andre', 'http://www.irisa.fr/michel.raynal/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Snow, C. R.', 'http://lsrwww.epfl.ch/schiper/')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Whiddett, Dick', 'http://www.cs.ncl.ac.uk/people/home.php?name=c.r.snow@ncl.ac.uk')" );

	stmt.executeUpdate (
		"INSERT INTO author VALUES ('Wilson, Gregory V.', 'http://www.third-bit.com/~gvwilson/')");





   //Create Table Publisher
	stmt.executeUpdate (
		"CREATE TABLE publisher (name varchar (255) PRIMARY KEY, address varchar (255), phone varchar (255), URL varchar (255))" );


   //Insert data into publisher

	stmt.executeUpdate (	
		"INSERT INTO publisher VALUES ('Benjamin/Cummings Publishing Co, Inc.', '390 Bridge Parkway, Redwood City, CA', 'http://www.aw-bc.com/', '415-402-2500')" );

	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Addison-Wesley Publishing Company', 'Reading, MA', 'http://www.aw-bc.com/', '617-848-7500')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('The MIT Press', 'MIT, Cambridge, MA  02142', 'http://mitpress.mit.edu/mail/home/', '800-405-1619')" );

	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Prentice-Hall', 'Englewood Cliffs, NJ 07632', 'http://vig.prenhall.com/', '201-236-7000')" );

	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('IEEE Computer Society Press', '10662 Los Vaqueros Circle, P. O. Box 3014, Los Adamitos,', 'http://www.wiley.com/ieeecs.htm', '877-762-2974')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Halsted Press, Wiley', '605 Third Avenue, New York, NY 10158', 'http://www.wiley.com/WileyCDA/', '877-762-2974')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Kluwer Academic Publishers', '101 Phillip Drive, Assinippi Park, Norwell, MA 02061', 'http://www.wkap.com/', '212-460-1575')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Cambridge University Press', 'University of Newcastle, New South Wales', 'http://www.cup.org/', '212-353-7500')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Halsted Press, Prentice-Hall', 'Englewood Cliffs, NJ 07632', 'http://vig.prenhall.com/', '201-236-7000')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Wiley & Sons', '111 River Street, Hoboken, NJ 07030', 'http://www.wiley.com/WileyCDA/', '877-762-2974')" );
	stmt.executeUpdate (
		"INSERT INTO publisher VALUES ('Adam Hilger', '242 Cherry Street, Philadelphia, PA 19106', 'http://www.iop.org', '555-555-5555')" );


   //Create Table book
	stmt.executeUpdate (
		"CREATE TABLE book (ISBN varchar (255) PRIMARY KEY, price varchar (255), title varchar (255), year varchar (255), URL varchar (255), publisher_name varchar (255) NOT NULL REFERENCES publisher (name))" );


   // Create table written_by
	stmt.executeUpdate (
		"CREATE TABLE written_by (ISBN varchar (255) NOT NULL REFERENCES book (ISBN), name varchar (255) NOT NULL REFERENCES author (name), PRIMARY KEY (ISBN, name))" );


  // Insert data into book and written_by

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-8053-0086-4', '88.35', 'Concurrent Programming: Principles and Practice', '1991', 'http://www.aw-bc.com/catalog/academic/product/0,1144,0805300864,00.html', 'Benjamin/Cummings Publishing Co, Inc.')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-8053-0086-4', 'Andrews, Gregory F.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-201-11721-5', '19.74', 'Programming Parallel Processors', '1987', 'http://portal.acm.org/citation.cfm?id=40470', 'Addison-Wesley Publishing Company')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-201-11721-5', 'Babb, Robert G., II, ed.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-262-03171-X', '50.75', 'How to Write Parallel Programs: A First Course', '1990', 'http://mitpress.mit.edu/catalog/item/default.asp?ttype=2&tid=5421', 'The MIT Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-262-03171-X', 'Carriero, Nicholas')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-262-03171-X', 'Gelernter, David')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-201-17435-9', '36.89', 'Concurrent Programming', '1988', 'http://www.bookhq.com/compare/0201174359.html', 'Addison-Wesley Publishing Company')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-201-17435-9', 'Gehani, Naraim')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-201-17435-9', 'McGettrick, Andrew D., eds.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-13-044628-9', '5.50', 'The Architecture of Concurrent Programs', '1977', 'http://www.bookhq.com/compare/0130446289.html', 'Prentice-Hall')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-13-044628-9', 'Hansen, Per Brinch')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-262-08205-5', '42.00', 'Data-Parallel Programming on MIMD Computers', '1991', 'http://mitpress.mit.edu/catalog/item/default.asp?ttype=28&tid=7125', 'The MIT Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-262-08205-5', 'Hatcher, Philip J.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-85274-811-6', '256.00', 'Parallel Computers 2', '1988', 'http://books.google.com/books?isbn=0852748116', 'Adam Hilger')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-85274-811-6', 'Hockney, R. W.')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-85274-811-6', 'Jesshop, C. R.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-13-153271-5', '39.21', 'Communicating Sequential Processes', '1985', 'http://research.microsoft.com/users/thoare/bibliography.aspx', 'Prentice-Hall')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-13-153271-5', 'Hoare, C. A. R.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-471-22982-2', '111.50', 'Parallel Computing on Heterogeneous Networks', '2003', 'http://www.wiley.com/WileyCDA/WileyTitle/productCd-0471229822.html', 'Wiley & Sons')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-471-22982-2', 'Lastovetsky, Alexy L.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-13-045923-2', '66.00', 'The Art of Parallel Programming', '1993', 'http://www.greenapplebooks.com/cgi-bin/mergatroid/63003.html', 'Prentice-Hall')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-13-045923-2', 'Lester, Bruce P.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-471-35831-2', '111.50', 'Parallel and Distributed Computing: A Survey of Models, Paradigms, and Approaches', '2001', 'http://www.wiley.com/WileyCDA/WileyTitle/productCd-0471358312.html', 'Wiley & Sons')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-471-35831-2', 'Leopold, Claudia')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-8186-5692-1', '54.00', 'Foundations of Parallel Programming: A Machine-Independent Approach', '1993', 'http://www.bookhq.com/compare/0818656921.html', 'IEEE Computer Society Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-8186-5692-1', 'Lewis, Ted G.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-471-82262-0', '14.95', 'Parallel Computing: Theory and Comparisons', '1987', 'http://www.bookhq.com/compare/0471822620.html', 'Halsted Press, Wiley')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-471-82262-0', 'Lipovski, G. J.')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-471-82262-0', 'Malek, M.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-20-114231-7', '29.33', 'Parallel Programming', '1987', 'http://www.bookhq.com/compare/0201142317.html', 'Addison-Wesley Publishing Company')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-20-114231-7', 'Perrott, R. H.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-89838-288-2', '7.98', 'Parallel Programming and Compilers', '1988', 'http://portal.acm.org/citation.cfm?id=534997', 'Kluwer Academic Publishers')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-89838-288-2', 'Polychronopoulos, Constantine D.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-262-18130-4', '14.65', 'Networks and Distributed Computation: Concepts, Tools, and Algorithms', '1988', 'http://mitpress.mit.edu/catalog/item/default.asp?ttype=2&tid=6588', 'The MIT Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-262-18130-4', 'Raynal, Michel')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-470-21346-9', '74.99', 'Concurrent Programming: Illustrated with Examples in Portal, Modula-2 and ADA', '1989', 'http://www.bookhq.com/compare/0470213469.html', 'Halsted Press, Wiley')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-470-21346-9', 'Schiper, Andre')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-521-33993-6', '31.99', 'Concurrent Programming', '1992', 'http://www.cambridge.org/us/catalogue/catalogue.asp?isbn=0521339936', 'Cambridge University Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-521-33993-6', 'Snow, C. R.')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-470-20979-8', '45.07', 'Concurrent Programming for software engineers', '1987', 'http://www.bookhq.com/compare/0470209798.html', 'Halsted Press, Prentice-Hall')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-470-20979-8', 'Whiddett, Dick')" );

		stmt.executeUpdate (
		"INSERT INTO book VALUES ('0-262-23186-7', '70.00', 'Practical Parallel Programming', '1995', 'http://mitpress.mit.edu/catalog/item/default.asp?ttype=28&tid=8185', 'The MIT Press')" );

		stmt.executeUpdate (
		"INSERT INTO written_by VALUES ('0-262-23186-7', 'Wilson, Gregory V.')" );



   //Create Table  warehouse
	stmt.executeUpdate (
		"CREATE TABLE warehouse (code varchar (255) PRIMARY KEY, address varchar (255), phone varchar (255))" );



   //Insert data into warehouse
	stmt.executeUpdate (
		"INSERT INTO warehouse VALUES ('ATL', '1775 Bridge Street, Atlanta, GA', '404-555-1212')" );
	stmt.executeUpdate (
		"INSERT INTO warehouse VALUES ('SFR', '15158 Dock Street, San Francisco, CA', '415-555-1212')" );
	stmt.executeUpdate (
		"INSERT INTO warehouse VALUES ('CHI', '151487 Stockard, Chicago, IL', '773-555-1212')" );






   //Create table stocks
	stmt.executeUpdate (
		"CREATE TABLE stocks (ISBN varchar (255) NOT NULL REFERENCES book (ISBN), code varchar (255) NOT NULL REFERENCES warehouse (code), number varchar (255), PRIMARY KEY (ISBN, code))" );



	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8053-0086-4', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8053-0086-4', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8053-0086-4', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-11721-5', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-11721-5', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-11721-5', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-03171-X', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-03171-X', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-03171-X', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-17435-9', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-17435-9', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-201-17435-9', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-044628-9', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-044628-9', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-044628-9', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-08205-5', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-08205-5', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-08205-5', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-85274-811-6', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-85274-811-6', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-85274-811-6', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-153271-5', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-153271-5', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-153271-5', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-22982-2', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-22982-2', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-22982-2', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-045923-2', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-045923-2', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-13-045923-2', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-35831-2', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-35831-2', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-35831-2', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8186-5692-1', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8186-5692-1', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-8186-5692-1', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-82262-0', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-82262-0', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-471-82262-0', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-20-114231-7', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-20-114231-7', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-20-114231-7', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-89838-288-2', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-89838-288-2', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-89838-288-2', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-18130-4', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-18130-4', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-18130-4', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-21346-9', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-21346-9', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-21346-9', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-521-33993-6', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-521-33993-6', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-521-33993-6', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-20979-8', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-20979-8', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-470-20979-8', 'CHI', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-23186-7', 'ATL', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-23186-7', 'SFR', '2')" );

	stmt.executeUpdate (
		"INSERT INTO stocks VALUES ('0-262-23186-7', 'CHI', '2')" );






 
           
      // Create a table in your database named customer.
	stmt.executeUpdate (
		"CREATE TABLE customer (email varchar (255) PRIMARY KEY, name varchar (255), address varchar (255), phone varchar (255))" ); 


   

      // Create a table in your database named shopping_basket.

	stmt.executeUpdate (
		"CREATE TABLE shopping_basket (email varchar (255) NOT NULL REFERENCES customer (email), ISBN varchar (255) NOT NULL REFERENCES book (ISBN), number varchar (255))" );
    
      // Get another statement object initialized
      // as shown.
      stmt = con.createStatement();


      // Input the XML customer data and parse it, 
      //	one customer at a time.
		try {
		  File file = new File("customers.xml");
		  DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		  DocumentBuilder db = dbf.newDocumentBuilder();
		  Document doc = db.parse(file);
		  doc.getDocumentElement().normalize();
		  NodeList nodeLst = doc.getElementsByTagName("customer");
	
		  for (int s = 0; s < nodeLst.getLength(); s++) {

		    Node fstNode = nodeLst.item(s);
		    
		    if (fstNode.getNodeType() == Node.ELEMENT_NODE) {
		  
		      Element fstElmnt = (Element) fstNode;
		      NodeList fstNmElmntLst = fstElmnt.getElementsByTagName("name");
		      Element fstNmElmnt = (Element) fstNmElmntLst.item(0);
		      NodeList fstNm = fstNmElmnt.getChildNodes();
		      String strName = new String ((fstNm.item(0)).getNodeValue());	     

		      String newstrName = strName.replaceAll("\'", "\\\\'");
		    

		      NodeList lstNmElmntLst = fstElmnt.getElementsByTagName("address");
		      Element lstNmElmnt = (Element) lstNmElmntLst.item(0);
		      NodeList lstNm = lstNmElmnt.getChildNodes();
		      String strAddress = new String ((lstNm.item(0)).getNodeValue());	     

		      String newstrAddress = strAddress.replaceAll("\'", "\\\\'");			

		      NodeList phoneNmElmntLst = fstElmnt.getElementsByTagName("phone");
		      Element phoneNmElmnt = (Element) phoneNmElmntLst.item(0);
		      NodeList phone = phoneNmElmnt.getChildNodes();
		      String strPhone = new String ((phone.item(0)).getNodeValue());	     

		      String newstrPhone = strPhone.replaceAll("\'", "\\\\'");	 

		      NodeList emailNmElmntLst = fstElmnt.getElementsByTagName("email");
		      Element emailNmElmnt = (Element) emailNmElmntLst.item(0);
		      NodeList email = emailNmElmnt.getChildNodes();
		      String strEmail = new String ((email.item(0)).getNodeValue());	     
		      String newstrEmail = strEmail.replaceAll("\'", "\\\\'");

		      stmt.executeUpdate ("INSERT INTO customer VALUES ('" + newstrEmail.trim() + "', '" + newstrName.trim() + "', '" + newstrAddress.trim() + "', '" + newstrPhone.trim() + "')" );
			
		    }

		  }
		  } catch (Exception e) {
		    e.printStackTrace();
		  }

         
        
	
      // Input customer order requests (in XML) and process


                try {
		  File file = new File("orders.xml");
		  DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		  DocumentBuilder db = dbf.newDocumentBuilder();
		  Document doc = db.parse(file);
		  doc.getDocumentElement().normalize();
		  NodeList nodeLst = doc.getElementsByTagName("order");
	
		  for (int s = 0; s < nodeLst.getLength(); s++) {

		    Node fstNode = nodeLst.item(s);
		    
		    if (fstNode.getNodeType() == Node.ELEMENT_NODE) {
		  
		      Element fstElmnt = (Element) fstNode;
		      NodeList fstNmElmntLst = fstElmnt.getElementsByTagName("email");
		      Element fstNmElmnt = (Element) fstNmElmntLst.item(0);
		      NodeList fstNm = fstNmElmnt.getChildNodes();
		      String strEmail = new String ((fstNm.item(0)).getNodeValue());	     

		      String newstrEmail = strEmail.replaceAll("\'", "\\\\'");
		    

		      NodeList lstNmElmntLst = fstElmnt.getElementsByTagName("ISBN");
		      Element lstNmElmnt = (Element) lstNmElmntLst.item(0);
		      NodeList lstNm = lstNmElmnt.getChildNodes();
		      String strISBN = new String ((lstNm.item(0)).getNodeValue());	     
			

		      NodeList numberNmElmntLst = fstElmnt.getElementsByTagName("number");
		      Element numberNmElmnt = (Element) numberNmElmntLst.item(0);
		      NodeList number = numberNmElmnt.getChildNodes();
		      String strNumber = new String ((number.item(0)).getNodeValue());	  

		      String sWHCode = new String();
		      Boolean bFound = false;;
		      rs = stmt.executeQuery (
				"SELECT * FROM stocks");
		      String sCode = new String();
		      while (rs.next() && !bFound)
			{
				bFound = false;
				String sISBN = rs.getString("ISBN");
			        sCode = rs.getString("code");
				String sNumber = rs.getString("number");

				
				if (sISBN.equals(strISBN.trim())) 
				{

					if (Integer.parseInt(sNumber)>= Integer.parseInt(strNumber.trim()))
					{
						stmt2.executeUpdate ("INSERT INTO shopping_basket VALUES ('" + newstrEmail.trim() + "', '" + strISBN.trim() + "', '" + strNumber.trim() + "')" );
						stmt2.executeUpdate (
							"UPDATE stocks SET number = '" + (Integer.parseInt(sNumber.trim()) - Integer.parseInt(strNumber.trim())) + "' WHERE ISBN = '" + sISBN.trim() + "' AND code = '" + sCode + "'" );
						bFound = true;	
		    			} // end if
				} //end if

		     }//end while
		    
		     
  		     if (!bFound) 
			{
				System.out.println("\nThe order by " + newstrEmail.trim() + " for " + strISBN.trim() + " is on back order. Please try again at some time in the future.");
			}// end if
		     else
			{ 

				System.out.println("\nThe order by " + newstrEmail.trim() + " for " + strISBN.trim() + " was successfully placed. Thank your for your business.");
				System.out.println("The order will ship from the  " + sCode.trim() + " warehouse.");
			} // end else

 		    }//end big if
                   }//End for

		} catch (Exception e) {
		          e.printStackTrace();
		}
  





    // Loop to print out billing Statements 
	  rs = stmt.executeQuery (
		"SELECT * FROM shopping_basket ORDER BY email" ) ;
	  String  sIsbn, sPrevEmail, sTitle, sCost;
	  Float totalCost = 0.00F;
	  Boolean firstTime = true;
	  sPrevEmail = "";
     	  System.out.println("\n**Display Billing Statements by email");
    	  System.out.println("\n------------------------------------------------------------------------------------------------------------------------------");
   	  System.out.println("\n\t ISBN\t\tNumber Ordered\t\tCost\t\tExtended Cost\t\tTitle");
	  
	  System.out.println("\n******************************************************************************************************************************");


          while (rs.next()) 
          {
	     String sEmail = rs.getString("email");
	     String sISBN = rs.getString("ISBN");
	     String sNum = rs.getString("number");
		rs2 = stmt2.executeQuery(
				"Select * FROM book WHERE ISBN = '" + sISBN.trim() + "'" );

	//	while (rs2.next()){
		rs2.next();
			sTitle = rs2.getString("title");
			sCost = rs2.getString("price");
		
		 
		if (!(sEmail.trim().equals(sPrevEmail)))
		{

			float fSandH = totalCost * .085F;
			totalCost = totalCost + fSandH;
			
			if (!firstTime){
				System.out.println("\n\t\t" + sPrevEmail +"\tShipping & Handling\t"+ nf.format(fSandH) +"\tTotal Cost\t" + nf.format(totalCost));
 				
				System.out.println("\n------------------------------------------------------------------------------------------------------------------------------");
				System.out.println("\n\n\t ISBN\t\tNumber Ordered\t\tCost\t\tExtended Cost\t\tTitle");
				System.out.println("\n******************************************************************************************************************************");
				
				totalCost = 0.00F;
				firstTime = false;
			}
			sPrevEmail = sEmail.trim();
			firstTime = false;

		}
	
		 totalCost = totalCost + (Float.parseFloat(sCost)* Integer.parseInt(sNum));
		 
		 System.out.println("\n\t" + sISBN + "\t\t" + sNum + "\t\t" + sCost + "\t\t" + nf.format((Float.parseFloat(sCost)* Integer.parseInt(sNum))) + "\t\t"  + sTitle  );
         
	    }//End while

	
	float fSandH = totalCost * .085F;
	totalCost = totalCost + fSandH;
	System.out.println("\n\t\t" + sPrevEmail +"\tShipping & Handling\t"+ nf.format(fSandH) +"\tTotal Cost\t" + nf.format(totalCost));

 
    	System.out.println("\n******************************************************************************************************************************");
	System.out.println("\n------------------------------------------------------------------------------------------------------------------------------");
	System.out.println("\n\t\t\t\tEnd Billing Statements");




      // Display all results

 	//Display all books
	  rs = stmt.executeQuery (
		"SELECT * FROM book ORDER BY ISBN" ) ;
	  System.out.println ("\n\n\n*************BOOK TABLE**********");
     	 
          while (rs.next()) 
          {
	     	String Isbn = rs.getString("ISBN");
	     	String price = rs.getString("price");
		String title = rs.getString("title");
		String year = rs.getString("year");
		String URL = rs.getString("URL");
		String pub = rs.getString("publisher_name");

		
		System.out.format ("%s\t %s\t %s\t %-80s %-70s %s\n" ,Isbn, price, year, title, URL, pub);
		
          }

     
	//Display all customers
	  rs = stmt.executeQuery (
		"SELECT * FROM customer" ) ;
	  System.out.println ("\n\n\n*************CUSTOMER TABLE**********");
     	 
          while (rs.next()) 
          {
	     	String email = rs.getString("email");
	     	String name = rs.getString("name");
		String address = rs.getString("address");
		String phone = rs.getString("phone");
       		name = name.replaceAll("\'", "\\\'");
		address = address.replaceAll("\'", "\\\\'");			
	      	

		System.out.format ("%-30s %-20s %-50s %s\n", email, name, address, phone);
		//System.out.println (email + "\t" + name +"\t\t" + address +"\t" + phone);
          }

        //Display all Shopping Baskets
	  rs = stmt.executeQuery (
		"SELECT * FROM shopping_basket" ) ;
	  System.out.println ("\n\n\n*************SHOPPING BASKET TABLE**********");
     	 
          while (rs.next()) 
          {
	     	String email = rs.getString("email");
	     	String Isbn = rs.getString("ISBN");
		String number = rs.getString("number");
			
		System.out.println (email + "\t\t" + Isbn +"\t" + number);
          }

	//Display all Warehouses
	
	  rs = stmt.executeQuery (
		"SELECT * FROM warehouse" ) ;
	  System.out.println ("\n\n\n*************WAREHOUSE TABLE**********");
     	 
          while (rs.next()) 
          {
	     	String code = rs.getString("code");
	     	String address = rs.getString("address");
		String phone = rs.getString("phone");
			
		System.out.println (code + "\t\t" + phone +"\t" + address);
          }

         //Display all items in Stocks
	  rs = stmt.executeQuery (
		"SELECT * FROM stocks" ) ;
	  System.out.println ("\n\n\n*************STOCKS TABLE**********");
     	 
          while (rs.next()) 
          {
	     	String code = rs.getString("code");
	     	String isbn = rs.getString("ISBN");
		String num = rs.getString("number");
			
		System.out.println (code + "\t\t" + isbn +"\t" + num);
          }

    // Delete the tables and close the connection
      // to the database
	try{
        stmt.executeUpdate("DROP TABLE book");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No book table to delete");
      }//end catch
  
  
     // Drop author
      try{
        stmt.executeUpdate("DROP TABLE author");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No author table to delete");
      }//end catch

      // Drop publisher
      try{
        stmt.executeUpdate("DROP TABLE publisher");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No publisher table to delete");
      }//end catch

      // Drop warehouse
      try{
        stmt.executeUpdate("DROP TABLE warehouse");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No warehouse table to delete");
      }//end catch

      // Drop customer
      try{
        stmt.executeUpdate("DROP TABLE customer");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No customer table to delete");
      }//end catch

      // Drop shopping_basket
      try{
        stmt.executeUpdate("DROP TABLE shopping_basket");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No shopping_basket table to delete");
      }//end catch

      // Drop stocks
      try{
        stmt.executeUpdate("DROP TABLE stocks");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No stocks table to delete");
      }//end catch


      // Drop written_by
      try{
        stmt.executeUpdate("DROP TABLE written_by");
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "No written_by table to delete");
      }//end catch


	con.close();
  }catch( Exception e ) {
      e.printStackTrace();

    }//end catch

  }//end main

}//end class Lab4A

