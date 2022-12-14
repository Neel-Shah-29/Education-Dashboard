#include "client.hpp"
/*
Handles the authentication of an existing user
*/
void login_user(user usr)
{
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    password = "";
    char ch = '\0';
    cin.ignore();
    while (1)
    {
        if (usr.rd_from_file)
            ch = getchar();
        else
            ch = getch_pwd();
        if (ch == '\n')
            break;
        password = password + ch;
        cout << "*" << flush;
    }
    cout << endl;
    password = encrypt(password);
    int len = username.length() + password.length() + 1;
    char sub_header[1024] = "SEND|0|";
    char *header = join_str_int(sub_header, len);
    string cred = username + delim + password;
    send_request(usr, header, cred, len);
}
/*
Handles the request for a new user
*/
void register_user(user usr)
{
    string username, password, password_temp;
    cout << "Username: ";
    cin >> username;
    bool valid = false;
    char ch;
    cin.ignore();
    while (!valid)
    {
        cout << "Password: " << flush;
        password = "";
        ch = '\0';
        while (1)
        {
            if (usr.rd_from_file)
                ch = getchar();
            else
                ch = getch_pwd();
            if (ch == '\n')
                break;
            password = password + ch;
            cout << "*" << flush;
        }
        cout << endl;
        cout << "Retype Password: " << flush;
        password_temp = "";
        ch = '\0';
        while (1)
        {
            if (usr.rd_from_file)
                ch = getchar();
            else
                ch = getch_pwd();
            if (ch == '\n')
                break;
            password_temp = password_temp + ch;
            cout << "*" << flush;
        }
        cout << endl;
        if (password_temp != password)
        {
            cout << "==========> " << flush;
            cout << "Passwords don't match" << endl;
        }
        else
            valid = true;
    }
    password = encrypt(password);
    int len = username.length() + password.length() + 1;
    char sub_header[1024] = "SEND|1|";
    char *header = join_str_int(sub_header, len);
    string cred = username + delim + password;
    send_request(usr, header, cred, len);
}