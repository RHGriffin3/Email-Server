//EmailServer.h
//Robert Griffin
//12/4/2015
//version 1.0
//
//This email server allows a user to create a username and password
//and communicate with other users through email stored in respective
//text files.



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

void MainMenu();
void Login();
void ShowInbox(std::string username, std::ostream & sout);
void LoggedInMenu(std::string username);
void RegisterNewUser();
void DeleteUser();
std::string GetUserFile(std::string username);
std::string GetInboxFile(std::string username);
std::string GetLine(std::istream & sin);
bool DoesUserExist(std::string username);
int GetInboxCount(std::string username);
std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg);
void SendEmail(std::string username);
std::string GetPassword(std::string username);

int main()
{
  MainMenu();
  return 0; // make sure your int main function returns an int
}

void MainMenu()
{
  std::string input = "";
  int menuItem = 0;

  while (input != "99")
  {
    std::cout << " 1) Login" << "\n";
    std::cout << " 2) Register new user" << "\n";
    std::cout << " 3) Delete user" << "\n";
    std::cout << "99) Exit" << "\n";
    std::cout << "Selection: ";

    std::cin >> input;
    try
    {
      menuItem = stoi(input);
      switch (menuItem)
      {
      case 1:
        Login();
        break;
      case 2:
        RegisterNewUser();
        break;
      case 3:
        DeleteUser();
        break;
      }
    }
    catch (std::invalid_argument & ia)
    {
      std::cout << "Bad option!" << std::endl;
      std::cout << ia.what() << std::endl;
    }
  }
}

void RegisterNewUser()
{
  //initialization of variables for username and password
	std::string userName;
	std::string passWord;

  //determining username and storing it in variable
	std::cout << "Enter new username: ";
	std::cin >> userName;
	
  //username must be at least 3 characters
	if (userName.length() < 3)
	{
		std::cout << "Username must be at least 3 characters!" << "\n";
		std::cout << "\n";
		return;
	}

  //initialization of bool variable to determine if user exists
	bool userExists = false;

  //call of function to determine if user exists
	userExists = DoesUserExist(userName);

  //if statement to check the bool variable
	if (userExists == true)
	{
		std::cout << "User already exists!" << "\n";
		std::cout << "\n";
		return;
	}
	else
	{
    //determining password and storing it in variable
		std::cout << "Enter new password: ";
		std::cin >> passWord;

    //password must be at least 6 characters
		if (passWord.length() < 6)
		{
			std::cout << "Password must be at least 6 characters" << "\n";
			std::cout << "\n";
			return;
		}

    //line to add space to the console
		std::cout << "\n";

    //initializing string to hold value of name of users file
		std::string userFile;

    //function call to determine name of users file
		userFile = GetUserFile(userName);

    //output to new file for user that includes storage of their username and password
		std::ofstream newUser;
		newUser.open (userFile);
		newUser << "username " << userName << "\n";
		newUser << "password " << passWord << "\n";

    //initializing string to hold value of name of users inbox
		std::string newUserInbox;

    //function call to determine name of users inbox file
		newUserInbox = GetInboxFile(userName);

    //output to new file new user's first welcome message to the email server
		std::ofstream userInbox;
		userInbox.open (userName + "-inbox.txt");
		userInbox << "#email" << "\n";
		userInbox << "mailservice" << "\n";
		userInbox << userName << "\n";
		userInbox << "Intro" << "\n";
		userInbox << "Hello, " << userName << "! Welcome to our service" << "\n";
	}
	
	

}

void Login()
{

  //initialization of username and password variables
	std::string userName;
	std::string passWord;

  //determine user's username and store in userName variable
	std::cout << "Enter username: ";
	std::cin >> userName;

  //bool variable to determine if user exists
	bool userExists = false;
  //function call to give a value to bool variable
	userExists = DoesUserExist(userName);

  //check to see if user exists
	if (userExists == true)
	{
		//if user exists, ask for password and store in passWord variable
		std::cout << "Enter password: ";
		std::cin >> passWord;

    //string to determine if passWord entered is equal to said user's password
		std::string userPass = GetPassword(userName);

    //only log in if both passwords are equal to eachother
		if (userPass == passWord)
		{

			std::cout << "\n";
			//call to logged in menu function
      LoggedInMenu(userName);
		}


	}
  //if user doesn't exist, return to main menu
	if (userExists == false)
	{
		std::cout << "User does not exist!" << "\n";
		std::cout << "\n";
		return;
	}



	
}

void ShowInbox(std::string username, std::ostream & sout)
{

  //initialization of input file stream 
	std::ifstream showInbox(username + "-inbox.txt");
	std::string line;
	
  //run while not at the end of the file
	while (!showInbox.eof())
	{
    //give line variable value of a line in the text file
		std::getline(showInbox, line);
    
    //if that line is the beginning of the email
		if (line == "#email")
		{
      //output the contents of the email
			std::getline(showInbox, line);
			sout << "From: " << line << "\n";
			std::getline(showInbox, line);
			sout << "To: " << line << "\n";
			std::getline(showInbox, line);
			sout << "Subject: " << line << "\n";
			std::getline(showInbox, line);
			sout << "Message: " << line << "\n";
			sout << "\n";
		}
	}
  //call back to the function this function was called from
	LoggedInMenu(username);
}

void LoggedInMenu(std::string username)
{
  //initialization of variable to hold amount of emails in inbox
	int emailCount;
  //function call to give value to variable
	emailCount = GetInboxCount(username);

  //variable to determine user's menu selection
	int selection;

  //menu interface
	std::cout << " 1) Inbox[" << emailCount << "]" << "\n";
	std::cout << " 2) Send Email" << "\n";
	std::cout << "99) Logout" << "\n";
	std::cout << "Selection: ";

	std::cin >> selection;

	std::cout << "\n";

  //switch case structure to determine what to do based on user's menu choice
	switch (selection)
	{
	case 1:
		ShowInbox(username, std::cout);
		break;

	case 2:
		SendEmail(username);
		break;

	case 99:
		MainMenu();
		break;

	default: std::cout << "\n";
		LoggedInMenu(username);
	}
}

void DeleteUser()
{
  //initialization of variable 
	std::string userName;

  //prompt to user to determine name of user to be deleted
	std::cout << "Who do you want to delete? ";
	std::cin >> userName;

  //deletion of all files related to userName
	std::remove((userName + ".txt").c_str());
	std::remove((userName + "-inbox.txt").c_str());

	std::cout << "\n";

}

std::string GetUserFile(std::string username)
{
  //creation of name of text file for username
	username = username + ".txt";
	return username;
}

std::string GetInboxFile(std::string username)
{
  //creation of name of text file for inbox of username
	username = username + "-inbox.txt";
	return username;
}

std::string GetLine(std::istream & sin)
{
  //variable to hold value of line in file
	std::string line;
  //giving value to variable 
	std::getline(sin, line);
  //return statement of variable holding value of a line
	return line;
}

bool DoesUserExist(std::string username)
{
  //initialization of input file stream
	std::ifstream fileCheck;
  //attempt to open file for username
	fileCheck.open(username + ".txt");

  //bool variable to check if file is open
	bool fileOpen;

	if (fileCheck.is_open() == true)
	{
		//if file is open, set variable to true
    fileOpen = true;
	}
	else
	{
		//if file did not open, set variable to false
    fileOpen = false;
	}
  //return bool variable
	return fileOpen;

}

int GetInboxCount(std::string username)
{
  //variable to hold amount of emails
	int count = 0;

	std::string line;
	std::ifstream inboxCount(username + "-inbox.txt");
  //until the file is at the end of file
	while (!inboxCount.eof())
	{
		std::getline(inboxCount, line); 
    //check to see if the line is the start of a new email
		if (line == "#email" || line == "#email ")
		{
      //if the line is the start of a new email, increment integer variable up by 1
			count = count + 1;
		}
	}
  //return amount of emails
	return count;
}

std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg)
{
  //variable to hold value of formatted email
	std::string formatted;

  //formatting of variable
	formatted = "#email\n"
		+ from + "\n"
		+ to + "\n"
		+ subject + "\n"
		+ msg + "\n";
  //return formatted email variable
	return formatted;
}

void SendEmail(std::string username)
{
  //variables to hold lines in the email
	std::string recipient;
	std::string subject;
	std::string message;



  //prompt to determine who to send email to
	std::cout << "Send email to who: ";
	std::cin >> recipient;

  //prompt to determine subject of email
	std::cout << "Subject: ";
	std::cin.ignore();
	std::getline(std::cin, subject);

  //prompt to determine message within email
	std::cout << "Message: ";
	std::cin.ignore(-1);
	std::getline(std::cin, message);

	std::cout << "Sending:";
	
  //variable to hold format of email
	std::string emailFormat = FormatEmailString(username, recipient, subject, message);

	std::cout << emailFormat;
	
  //filestream to add to recipient's inbox
	std::fstream eMail;
	
  //append to recipient's inbox rather than overwrite
	eMail.open(recipient + "-inbox.txt", std::fstream::app);
	
	//output to recipient's inbox
	eMail << emailFormat;
	eMail.close();

	std::cout << "\n";
	LoggedInMenu(username);
}

std::string GetPassword(std::string username)
{
  //input file stream for username
	std::ifstream userFile(username + ".txt");
	std::string firstLine;
  //get first line of file which is useless in this case, we just want to get past it
	std::getline(userFile, firstLine);

  //until end of file
	while (!userFile.eof())
	{
		std::string line;

    //get line of file that contains the password
		std::getline(userFile, line);

		std::stringstream lineReader(line);

		std::string pass, password;
    //read the line and give values to two variables, one for each word
		lineReader >> pass >> password;
    //return second variable, the only one relevant
		return password;
	}
	

}
