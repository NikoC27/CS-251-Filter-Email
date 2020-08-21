#include <iostream>
#include <string>
#include "ourvector.h"
#include <fstream>
using namespace std;

/* ----------------------------- Project 1 -----------------------------------
 * 
 * CS 251 - Professor Hummel
 * 
 * Date: 1/20/2020
 * 
 * Created by Niko Castellana
 *
 * --------------------------------------------------------------------------- */
//
// ----------------------------------------------------------------------------
//
/* The purpose of this program is to load, check and filter through a list
 * of emails. The user may load and display a list of spam emails from the 
 * spam lists provided. Additionally the user may check an email name to see
 * if that email is spam from one of the spam files. The filter function is
 * used, once a spam email list is loaded, to compare a list of emails to
 * a spam list, and counts the number of emails processed and nonspam emails. */
//
// ----------------------------------------------------------------------------
// 
/* This function passes in the vector and the name of the spam file the user 
 * is trying to load. Once the spam file is opened, every line in that file
 * is pushed back to the email vector. The number of spam entries in the is 
 * counted and printed once the file is loaded. */
//
// Start of loadSpamList()
// 
void loadSpamList(ourvector<string>& email, string file)
{ 
	int emailCounter = 0;      // Counts the number of emails in file
	email.clear();             // Clear the vector 
	ifstream infile;           // Read from file
	infile.open(file);         // Open the file
	string line;               // Lines of the file
    
	if (!infile.is_open())
	{
		cout << "**Error, unable to open " << "'" << file << "'" 
                << endl;
		return;
	}

	while (!infile.eof())
    	{
		getline(infile, line);

		if (line != "")              // Check for blank lines
		{
			emailCounter++;          // Increment number of emails in spam file
			email.push_back(line);   // Push back lines to vector
		}
   	 }

	infile.close(); // Close the file

	cout << "Loading " << "'" << file << "'" << endl;              
	cout << "# of spam entries: " << emailCounter << endl << endl; 

} // End of loadSpamList()
//
// ----------------------------------------------------------------------------
// 
/* This function displays the spam list once it is loaded, and will not print
 * a spam list until the user loads one. It knows a when a spam list is not
 * loaded by checking the elements in the email vector */
//
// Start of displaySpamList()
// 
void displaySpamList(ourvector<string> email)
{
	for (int i = 0; i < email.size(); i++)
	{
		if (email.size() == 0)
		{
			;                             // Do nothing if the vector is empty 
		}
		else
		{
			cout << email.at(i) << endl;  // Display spam list
		}

	}
    
	cout << endl;
    
} // End of displaySpamList()
//
// ----------------------------------------------------------------------------
// 
/* This function performs binary search on the email vector to check if the email
 * entered by the user matches a spam email in the vector. The function also helps
 * filter through email lists to separate the real emails from the spam emails */
//
// Start of binarySearch()
bool binarySearch(ourvector<string> email, string emailName)
{
	int low = 0;
	int high = email.size() - 1;     // Accounts for out of bounds
	int mid = 0;

	while (low <= high) 
	{
		mid = low + (high - low) / 2; 
		string midElement = email[mid]; // Find middle of the vector

		if (midElement > emailName) 
        	{
			high = mid - 1;
		}
		else if (midElement < emailName) 
        	{
			low = mid + 1;
		}

		else 
       		 {
			return true;
		}
	
	}
	
	return false;
	
} // End of binarySearch()
//
// ----------------------------------------------------------------------------
// 	
/* This function takes in the email name entered by the user and checks if that
 * email is in the spam vector. String concatenation is used on the email name
 * entered by the user to change the notation. Example: fred@yahoo.com changes
 * to yahoo.com:fred. This method is used to ensure that the notation the user
 * entered is the same as the notation in the spam files. */
//
// Start of checkForSpam()
//
bool checkForSpam(ourvector<string> email, string emailName) 
{
	string domain = "";             // String after the @ symbol
	string userName = "";           // String before the @ symbol
	string stringName = "";         // String used after concatenation
	string stringNameStar = "";     // String used after concatenation accounts for "*'
	size_t position = 0;            // Index of string

	position = emailName.find('@'); // Update position to index of character
	userName = emailName.substr(0, position); // Index: 0 - @
	domain = emailName.substr(position + 1 , string :: npos); // Index @ + 1 to the last index
	stringName = domain + ":" + userName; // Email name changed
	stringNameStar = domain + ":" + "*";  // Email name changed and * accounted for
    
	if (binarySearch(email, stringName) == true || binarySearch(email, stringNameStar) == true)
    	{
       		return true; // Spam found in search
    	}
    
    	else
    	{
        	return false; // Spam not found in search
	}

} // End of checkForSpam()
//
// ----------------------------------------------------------------------------
// 
/* This function opens the file name entered by the user and reads through the 
 * list of emails. It calls the checkForSpam() function to separate the spam from
 * the non-spam emails. The number of emails processed and non-spam emails are
 * displayed after the file has been filtered. Also, the non-spam emails are written
 * to a new file that the user types in. */
//
// Start of filterEmailList()
// 
void filterEmailList(ourvector<string> email, string file, string outFile)
{
	int messageID = 0;            // ID number of the email
    	string emailAddress = "";     // Email Address
    	string subject = "";          // Subject written after email
    	int emailsProcessed = 0;      // Counts number of emails processed
    	int nonspamEmails = 0;        // Counts number of non-spam emails
    
    	ifstream infile;              // Read from file
	infile.open(file);            // Open file
    
	if (!infile.is_open())
	{
		cout << "**Error, unable to open " << "'" << file << "'" 
             	<< endl;
		return;
	}
    
   	ofstream oFile;               // Write to output file
 	oFile.open(outFile);          // Open output file
    
	if (!oFile.good())
	{
        	return;
   	}
    
	infile >> messageID;          // Gets input for ID number
    	infile >> emailAddress;       // Gets input for email address
    	getline(infile, subject);     // Gets input for the subject
    
	while (!infile.eof())
	{
        	if (!infile.fail())
        	{
            		// Prints line to file when there is no spam 
             		if (!checkForSpam(email, emailAddress)) 
           		{    
                 		oFile << messageID << " " << emailAddress << " " << subject << endl;
                		nonspamEmails++; // Increment non-spam emails
           		}
        	}	
        
    		infile >> messageID;       // Update input 
       	 	infile >> emailAddress;
		getline(infile, subject);
        	emailsProcessed++;         // Increment total emails 
        
	} // End of loop

	infile.close(); // Close the input file
    	oFile.close();  // Close the output file
    
    	cout << "# emails processed: " << emailsProcessed << endl;
    	cout << "# non-spam emails:  " << nonspamEmails << endl << endl;
    
} // End of filterEmailList()
// 
// ----------------------------------------------------------------------------
// 
// Start of main()
//
int main()
{
	string userInput = "";       // Prompt user to load, display, check or filter
	string file = "";            // Prompt user for file name
	string emailName = "";       // Prompt user for email name
    	string outFile = "";         // Prompt user for output file
	ourvector<string> email;     // Declare email vector
    
	cout << "** Welcome to spam filtering app **" << endl << endl;
	cout << "Enter command or # to exit> ";  // Prompt user for input
	cin >> userInput;
    
	while (userInput != "#")
	{
		if (userInput == "load")
		{
			cin >> file;
			loadSpamList(email, file);  // Load spam list
		}
		else if (userInput == "display")
		{
			displaySpamList(email);  // Display spam list
		}
		else if (userInput == "check")
		{
			cin >> emailName;
            
               		// Check for spam
			if (checkForSpam(email, emailName) == true)
           		 {
                		// Email is spam
                		cout << emailName << " is spam" << endl << endl; 
            		 }
            		else
           	 	{
                		// Email is not spam
                		cout << emailName << " is not spam" << endl << endl;
            		}
            
		}
        	else if (userInput == "filter")
        	{
          		cin >> file;
            		cin >> outFile;
            		filterEmailList(email, file, outFile);  // Filter through emails
       		}
        	else
        	{
            		cout << "**invalid command" << endl << endl; // Check for invalid commands
        	}
		
		cout << "Enter command or # to exit> ";  // Update for user input
		cin >> userInput;
        
	
	} // End of loop

	return 0;
    
} // End of main()
//
// ----------------------------- End of Program 1 ------------------------------------
