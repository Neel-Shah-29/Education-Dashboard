#include "client.hpp"
// Handles the create post feature of the instructor
void create_post(user usr)
{
    // Modify this in accordance with types
    string category, assignment_name, deadline,
        desc;
    vector<string> file_names;
    int type, num_files;
    cout << "Type [Press 1/2]: " << flush;
    cin >> type;
    cout << "Category: " << flush;
    cin >> category;
    cout << "Name: " << flush;
    cin >> assignment_name;
    if (type == 1)
    {
        cout << "Deadline: " << flush;
        cin >> deadline;
    }
    cin.ignore();
    cout << "Description: " << flush;
    getline(cin, desc);
    cout << "Number of Files: " << flush;
    cin >> num_files;
    for (int i = 0; i < num_files; i++)
    {
        cout << "Enter File ";
        cout << i + 1;
        cout << ": ";
        cout << flush;
        string temp;
        cin >> temp;
        file_names.push_back(temp);
    }
    string to_send;
    if (type == 1)
    {
        to_send = to_string(type) + delim + category + delim + assignment_name + delim + deadline +
                  delim + desc + delim + to_string(num_files);
        for (int i = 0; i < num_files; i++)
        {
            to_send = to_send + delim +
                      file_names[i];
        }
    }
    else
    {
        to_send = to_string(type) + delim +
                  category + delim + assignment_name + delim +
                  desc + delim + to_string(num_files);
        for (int i = 0; i < num_files; i++)
        {
            to_send = to_send + delim +
                      file_names[i];
        }
    }
    int len = to_send.length();
    char sub_header[1024] = "SEND|0|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
    if (num_files > 0)
    {
        get_response(usr);
    }
    for (int i = 0; i < num_files; i++)
    {
        upload_file(usr);
    }
}
// Handles the initiation of chat session by the user
void chat_session(user usr)
{
    string code;
    cout << "Choose a code(under 50000): " << flush;
    cin >> code;
    int p_code = stoi(code);
    string to_send = code;
    int len = to_send.length();
    char sub_header[1024] = "ASK|8|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
    sleep(1);
    pthread_t chat_thread;
    if (pthread_create(&chat_thread, NULL,
                       chat_client, &p_code) != 0)
    {
        perror("thread creation error");
        return;
    }
    pthread_join(chat_thread, NULL);
    strcpy(sub_header, "ASK|9|");
    header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
}
// Uploads attachments posted by the Instructor
void upload_file(user usr)
{
    string path;
    cout << "File Path: " << flush;
    cin >> path;
    int len = get_file_size(path);
    if (len == -1)
        return;
    char sub_header[1024] = "SEND|0|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, "", 0);
    send_file(usr, path, len);
}
// Displays all submissions to the Instructor
void view_all_submissions(user usr)
{
    string category, assignment_name;
    cout << "Category: " << flush;
    cin >> category;
    cout << "Assignment Name: " << flush;
    cin >> assignment_name;
    string to_send = category + delim +
                     assignment_name;
    int len = to_send.length();
    char sub_header[1024] = "ASK|1|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
}
// Received commands to isplay a particular student's submission
void view_particular_submission(user usr)
{
    string category, assignment_name, student;
    cout << "Category: " << flush;
    cin >> category;
    cout << "Assignment Name: " << flush;
    cin >> assignment_name;
    cout << "Student Name: " << flush;
    cin >> student;
    string to_send = category + delim +
                     assignment_name + delim + student;
    int len = to_send.length();
    char sub_header[1024] = "ASK|2|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
}
// Handles th view post feature of both Instructor
and Client void view_post(user usr)
{
    string type, category, assignment_name;
    cout << "Type: " << flush;
    cin >> type;
    cout << "Category: " << flush;
    cin >> category;
    cout << "Assignment Name: " << flush;
    cin >> assignment_name;
    string to_send = type + delim + category +
                     delim + assignment_name;
    int len = to_send.length();
    char sub_header[1024] = "ASK|0|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
}
// Handles the download file request of the
Instructor void download_file(user usr)
{
    string category, assignment_name, student,
        file_name;
    cout << "Category: " << flush;
    cin >> category;
    cout << "Assignment Name: " << flush;
    cin >> assignment_name;
    cout << "Student Name: " << flush;
    cin >> student;
    cout << "File Name: " << flush;
    cin >> file_name;
    string to_send = category + delim +
                     assignment_name + delim + student + delim +
                     file_name;
    int len = to_send.length();
    char sub_header[1024] = "ASK|3|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, to_send, len);
    get_response_file(usr, file_name);
}
// Handles the request for displaying the list of
people joined void get_people(user usr)
{
    int len = 0;
    char sub_header[1024] = "ASK|4|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, "", len);
}
// Handles the request for displaying the list of
posts shared by instructor void get_classwork(user usr)
{
    int len = 0;
    char sub_header[1024] = "ASK|5|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, "", len);
}
// Used to exit a class and going back to the
outline view void exit_class(user usr)
{
    int len = 0;
    char sub_header[1024] = "ASK|7|";
    char *header = join_str_int(sub_header, len);
    send_request(usr, header, "", len);
}