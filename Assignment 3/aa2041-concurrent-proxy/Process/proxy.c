/*
 * proxy.c - Concurrent Web proxy - Processes
 *
 * Course Name: 14:332:456-Network Centric Programming
 * Assignment 3
 * Student Name: Ashwin Anand
 *
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */

// class that includes the methods in a separate class
#include "csapp.h"


// global variable that can be used for all methods
int log_file_directory_value;


// These function's are to be included in csapp.c and csapp.h later on
ssize_t read_fail_method(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t reading_failing_warning_method(int fd, void *ptr, size_t nbytes);
void writing_failing_method(int fd, void *usrbuf, size_t n);

// calling the three methods
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
int parsing_url(char *url, char *target_addr, char *path, int  *port);
void process_method(int connfd, struct flock fileLock, struct sockaddr_in clientaddr);


// main method
int main(int argc, char **argv)
{

	if (argc != 2)
    {
		fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
		exit(0);
	}

	// signal hander for processes that are zombies
	struct sigaction sig_child_action =
	{
		.sa_handler = SIG_DFL,
		.sa_flags = SA_NOCLDWAIT
	};

	// sig action method for sig child
	sigaction(SIGCHLD,&sig_child_action,NULL);
	signal(SIGPIPE, SIG_IGN);

	// listens to file directory for connections
	int listen_file_directory = Open_listenfd(atoi(argv[1]));

	// mode for writing into file later on
	mode_t mode_type = S_IRWXO| S_IRWXG | S_IRWXU;
	log_file_directory_value = open("PLog.txt",O_WRONLY | O_APPEND | O_CREAT,mode_type);


	// variables initalized before while loop
	int client_len;
	struct sockaddr_in clientaddr;
	int connection_file_directory;
	int server_file_directory;
	char *http_request;
	char *http_request_url;
	char *http_request_url_end;
	char *rest_of_http_request;
	int http_request_length;
	int http_response_len;
	char host_name[MAXLINE];
	char path_name[MAXLINE];
	int server_port;
	char log_entry_into_text[MAXLINE];

    struct flock fileLock;
		fileLock.l_type = F_WRLCK;
		fileLock.l_whence = SEEK_SET;
		fileLock.l_start = 0;
		fileLock.l_len = 0;

	// rio object for written and read
	rio_t rio_object;
	char buffer_object[MAXLINE];
	int error_count = 0;

    // While loop to continously listen to requests
	while (1)
    {
		error_count = 0;
		client_len = sizeof(clientaddr);
		connection_file_directory = Accept(listen_file_directory, (SA *)&clientaddr, &client_len);
		int child_Pid_value;

		// if loop to create a child pid value
		if((child_Pid_value = fork()) == 0)
    {
		int a = 0;
		int b = 0;
		int r_Factor;

		http_request = (char *)Malloc(MAXLINE);
		http_request[0] = '\0';
		r_Factor = 2;
		http_request_length = 0;
		Rio_readinitb(&rio_object, connection_file_directory);

		// while loop to go through the entire request
		while (1)
		{
			// if loop to read the request and will produce error if there is one in reading
			b = read_fail_method(&rio_object, buffer_object, MAXLINE);
			if (b<= 0)
			{
			error_count = 1;
			close(connection_file_directory);
			free(http_request);
			break;
			}

			// if loop to make sure that the http request length is more than defined maxline
			if (http_request_length + b + 1 > MAXLINE)
			{
				Realloc(http_request, MAXLINE*r_Factor++);
			}
			strcat(http_request, buffer_object);
			http_request_length += b;

			// if there is a space between http request then it exits the loop
			if (strcmp(buffer_object, "\r\n") == 0)
			{
				break;
			}
		}

			// if loop to make sure there is a GET request from the server
			if (strncmp(http_request, "GET ", strlen("GET ")))
            {
				close(connection_file_directory);
				free(http_request);
				break;
			}
			http_request_url = http_request + 4;
			http_request_url_end = NULL;

			// for loop get the url from the request
			for (a = 0; a < http_request_length; a++)
			{
				if (http_request_url[a] == ' ')
				{
					http_request_url[a] = '\0';
					http_request_url_end = &http_request_url[a];
					break;
				}
			}

			// if loop go through entire url for end point and if found then closes and breaks the loop
			if ( a == http_request_length )
            {
				close(connection_file_directory);
				free(http_request);
				break;
			}

			// if loop to ensure url is a http and not anyhting else
			if (strncmp(http_request_url_end + 1, "HTTP/1.1\r\n", strlen("HTTP/1.1\r\n")) && strncmp(http_request_url_end + 1, "HTTP/1.0\r\n", strlen("HTTP/1.0\r\n")))
            {
				close(connection_file_directory);
				free(http_request);
				break;
			}
			rest_of_http_request = http_request_url_end + strlen("HTTP/1.0\r\n") + 1;


			// if loop that will go through the entirety of the method and breakup the url into different parts
			int temp_parse_url =parsing_url(http_request_url, host_name, path_name, &server_port);
			if (temp_parse_url < 0)
            {
				close(connection_file_directory);
				free(http_request);
				break;
			}

			server_file_directory = open_clientfd(host_name, server_port);

			// if loop that will take the request to the server
			if (server_file_directory < 0)
			{
				free(http_request);
				break;
			}


			// writing failing method for server file directory
			writing_failing_method(server_file_directory, "GET /", strlen("GET /"));
			writing_failing_method(server_file_directory, path_name, strlen(path_name));
			writing_failing_method(server_file_directory, " HTTP/1.0\r\n", strlen(" HTTP/1.0\r\n"));
			writing_failing_method(server_file_directory, rest_of_http_request, strlen(rest_of_http_request));

            Rio_readinitb(&rio_object,server_file_directory);
			http_response_len = 0;

			// while loop that will take in server response and sent it to the client
			while( (b = reading_failing_warning_method(server_file_directory, buffer_object, MAXLINE)) > 0 )
			{
				http_response_len += b;

				writing_failing_method(connection_file_directory, buffer_object, b);
				bzero(buffer_object, MAXLINE);
			}

			// lock the file with pid value
			fileLock.l_pid = getpid();

			fcntl(log_file_directory_value,F_SETLKW,&fileLock);

			// write it into the PLog text file
			format_log_entry(log_entry_into_text, &clientaddr, http_request_url, http_response_len);
			char entry[strlen(log_entry_into_text) + 8];
			char response_length[6];
			strcpy(entry,log_entry_into_text);
			strcat(entry," ");
			sprintf(response_length,"%d\n",http_response_len);
			strcat(entry,response_length);
			write(log_file_directory_value,entry,strlen(entry));

			// close log file directory
			close(log_file_directory_value);
			fileLock.l_type = F_UNLCK;
			fcntl(log_file_directory_value,F_SETLK,&fileLock);

			// close all the file directorys and http request
			close(log_file_directory_value);
			close(connection_file_directory);
			close(server_file_directory);
			free(http_request);
			exit(0);
		}
	}

	// exit the method
    exit(0);
}


/*
 * read_fail_method - on read failing print a message
 */
ssize_t read_fail_method(rio_t *readpoint, void *userbuffer, size_t maxlength)
{
    ssize_t a;

    if ((a = rio_readlineb(readpoint, userbuffer, maxlength)) < 0) {
	printf("Warning: reading failed\n");
	return 0;
    }
    return a;
}

/*
 * reading_failing_warning_method - on read faling print a message
 */
ssize_t reading_failing_warning_method(int filedir, void *pointer, size_t numbytes)
{
    ssize_t a;

    if ((a = rio_readn(filedir, pointer, numbytes)) < 0) {
	printf("Warning: reading failed\n");
	return 0;
    }
    return a;
}

/*
 * writing_failing_method - writing failing
 */
void writing_failing_method(int filedir, void *userbuffer, size_t b)
{
    if (rio_writen(filedir, userbuffer,b) != b) {
	printf("Warning: writing failed.\n");
    }
}

/*
 * parse_uri - URI parser
 *
 * This method basically takes in the HTTP GET request, host name , path name, port
 * all extracted will return the int value of the url. The memory has already been allocated
 */

int parsing_url(char *url, char *host_name, char *path_name, int *port_number)
{
    // if loop that will compare url to see if it contains http:// in 7 char objects
    if (strncasecmp(url, "http://", 7) != 0)
    {
	    host_name[0] = '\0';
	    return -1;
    }

    char *address_host_begin = url + 7;
    char *address_host_end = strpbrk(address_host_begin, " :/\r\n\0");
    int address_length = address_host_end - address_host_begin;
    strncpy(host_name, address_host_begin, address_length);
    host_name[address_length] = '\0';

    // default port number for any server is 80
    *port_number = 80;

    // i floop that will take the port number from the address
    if (*address_host_end == ':')
	{
        *port_number = atoi(address_host_end + 1);
    }
    char *address_path_begin = strchr(address_host_begin, '/');

    // if loop that take the address path if the path is null
    if (address_path_begin == NULL)
    {
	    path_name[0] = '\0';
    }

    // else loop that will go here if the address path is full
    else
    {
	address_path_begin++;
	strcpy(path_name, address_path_begin);
    }

    // return statement
    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (url), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
		      char *url, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s", time_str, a, b, c, d, url,size);
}


void process_method(int connfd, struct flock file_locking, struct sockaddr_in client_address)
{

    // intializing variables before while loop
    rio_t rio_object;
    char buffer_object[MAXLINE];
    int count_error = 0;
     int connection_file_directory = connfd;
    int server_file_directory;
    char *http_request_url;
    char *http_request_url_end;
    char *rest_of_http_request;
    int http_response_len;
    int a, b;
    char address_host_name[MAXLINE];
    char address_path_name[MAXLINE];
    int server_port_number;
    char log_entry_object[MAXLINE];
    char *http_request = (char *)Malloc(MAXLINE);
    http_request[0] = '\0';
    int r_factor = 2;
    int http_request_len = 0;
    Rio_readinitb(&rio_object, connection_file_directory);

    // loop that its infinte for unlimited request
    while (1)
    {
        // if loop that will go though the buffer and any error happense will cause the system to have an error
        if ((b = read_fail_method(&rio_object, buffer_object, MAXLINE)) <= 0)
        {
            count_error = 1;
            close(connection_file_directory);
            free(http_request);
            break;
        }


        if (http_request_len + b + 1 > MAXLINE)
        {
            Realloc(http_request, MAXLINE*r_factor++);
        }

        strcat(http_request, buffer_object);
        http_request_len += b;


        if (strcmp(buffer_object, "\r\n") == 0)
        {
            break;
        }

    }

    // any error count will exit the program
    if (count_error)
	{
        exit(0);
    }

    // makes sure that there is a get
    if (strncmp(http_request, "GET ", strlen("GET ")))
    {
        close(connection_file_directory);
        free(http_request);
	    exit(0);

    }
    http_request_url = http_request + 4;
    http_request_url_end = NULL;

    // for loop thath will through request length for url
    for (a = 0; a < http_request_len; a++)
    {
        // looking for spaces in request
        if (http_request_url[a] == ' ')
        {
            http_request_url[a] = '\0';
            http_request_url_end = &http_request_url[a];
	        exit(0);

        }
    }

    // if loop that till go through all request to close connection
    if ( a == http_request_len )
    {
        close(connection_file_directory);
        free(http_request);
	    exit(0);
    }

    // if loop that find the HTTP in request url
    if (strncmp(http_request_url_end + 1, "HTTP/1.0\r\n", strlen("HTTP/1.0\r\n")) && strncmp(http_request_url_end + 1, "HTTP/1.1\r\n", strlen("HTTP/1.1\r\n")))
    {
        close(connection_file_directory);
        free(http_request);
	    exit(0);

    }

    rest_of_http_request = http_request_url_end + strlen("HTTP/1.0\r\n") + 1;

    // if loop that will go through parsing of url to find important info form the url
    if (parsing_url(http_request_url, address_host_name, address_path_name, &server_port_number) < 0)
    {
        close(connection_file_directory);
        free(http_request);
	    exit(0);

    }

    // if loop that the server will continouly listen for requests
    if ((server_file_directory = open_clientfd(address_host_name, server_port_number)) < 0)
    {

        free(http_request);
        exit(0);

    }

    // writes to the file through path name and http request
    writing_failing_method(server_file_directory, "GET /", strlen("GET /"));
    writing_failing_method(server_file_directory, address_path_name, strlen(address_path_name));
    writing_failing_method(server_file_directory, " HTTP/1.0\r\n", strlen(" HTTP/1.0\r\n"));
    writing_failing_method(server_file_directory, rest_of_http_request, strlen(rest_of_http_request));
    Rio_readinitb(&rio_object, server_file_directory);
    http_response_len = 0;

    // while loop that will read the server and write it to the file
    while( (b = reading_failing_warning_method(server_file_directory, buffer_object, MAXLINE)) > 0 ) {
        http_response_len += b;
        writing_failing_method(connection_file_directory, buffer_object, b);
        bzero(buffer_object, MAXLINE);
    }


    file_locking.l_pid = getpid();
    fcntl(log_file_directory_value,F_SETLKW,&file_locking);

    // writing to text file about browsing history of http sites
    format_log_entry(log_entry_object, &client_address, http_request_url, http_response_len);
    char entry[strlen(log_entry_object) + 8];
    char http_response_length[6];
    strcpy(entry,log_entry_object);
    strcat(entry," ");
    sprintf(http_response_length,"%d\n",http_response_len);
    strcat(entry,http_response_length);
    write(log_file_directory_value,entry,strlen(entry));
    close(log_file_directory_value);

    // locks and unlocks the files and closes connections
    file_locking.l_type = F_UNLCK;
    fcntl(log_file_directory_value,F_SETLK,&file_locking);
    close(connection_file_directory);
    close(server_file_directory);
    free(http_request);

}
