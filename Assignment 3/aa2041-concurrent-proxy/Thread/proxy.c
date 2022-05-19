/*
 * proxy.c - Concurrent Web proxy - Threads
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


// global variables that can be used for all methods
FILE *text_log;
pthread_mutex_t lock_the_file;
pthread_mutex_t lock_the_function;

// functions that i am using
void writing_failing_method(int fd, void *usrbuf, size_t n);
ssize_t read_failing_line_method(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t read_failing_method(int fd, void *ptr, size_t nbytes);

// function calls
int parse_url_method(char *uri, char *target_addr, char *path, int  *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);

// structure for argument for sockets and connection
struct argument_structure
{
	struct sockaddr_in *client_address_pointer;
	int connection_fd;
};

// thread process method
void *thread_process_method(void *args)
{
	// struct for args and client sockets
	struct  argument_structure *args_of_threads = (struct argument_structure *)args;
	struct sockaddr_in *clientaddrPtr;
	int connection_file_directory;
	clientaddrPtr = args_of_threads->client_address_pointer;
	connection_file_directory = args_of_threads->connection_fd;

	// initalizing variables before while loop
    char host_name[MAXLINE];
    char path_name[MAXLINE];
    char text_log_entry_object[MAXLINE];
    rio_t rio_object;

    char buffer_object[MAXLINE];
    int count_of_error = 0;
	char *http_request = (char *)Malloc(MAXLINE);
    http_request[0] = '\0';
    int r_factor = 2;
    int http_response_length = 0;
	pthread_mutex_lock(&lock_the_function);
    Rio_readinitb(&rio_object, connection_file_directory);
	pthread_mutex_unlock(&lock_the_function);
	int server_file_directory;
	char *http_request_url;
    char *http_request_url_end;
	char *rest_of_http_request;
	int http_request_length;
    int a, b;
	int server_port_num;

	// runs infinitely
    while (1)
	{

		b = read_failing_line_method(&rio_object, buffer_object, MAXLINE);

		// if loop to read the url and request from buffer
        if (b <= 0)
		{
        	count_of_error = 1;
            close(connection_file_directory);
            free(http_request);
            break;
        }

        // if loop to make sure request is bigger than value of maxline
        if (http_request_length + b + 1 > MAXLINE)
		{
            Realloc(http_request, MAXLINE*r_factor++);
		}

        strcat(http_request, buffer_object);
        http_request_length += b;

        // if loop that will break if theres a break in the url
        if (strcmp(buffer_object, "\r\n") == 0)
        {
			break;
		}
    }

	// if errors are there then leave the thread
    if (count_of_error)
	{
		pthread_exit(NULL);
	}

	// if loop that will maake sur etheer is a get request
    if (strncmp(http_request, "GET ", strlen("GET ")))
	{

        close(connection_file_directory);
        free(http_request);
		pthread_exit(NULL);
    }

	http_request_url = http_request + 4;

    http_request_url_end = NULL;

	// for loop that will through the reguest length and shorten url by taking out blanks
	for (a = 0; a < http_request_length; a++)
	{
       	if (http_request_url[a] == ' ')
		{
            http_request_url[a] = '\0';
            http_request_url_end = &http_request_url[a];
            break;
        }
    }

	// if loop that matches a with request length and closes and leaves the file
    if ( a == http_request_length )
	{

        close(connection_file_directory);
        free(http_request);
		pthread_exit(NULL);
    }

	// if loop that will check if HTTP is at the request url
    if ( strncmp(http_request_url_end + 1, "HTTP/1.1\r\n", strlen("HTTP/1.1\r\n")) && strncmp(http_request_url_end + 1, "HTTP/1.0\r\n", strlen("HTTP/1.0\r\n")))
	{

        close(connection_file_directory);
        free(http_request);
		pthread_exit(NULL);
    }

    rest_of_http_request = http_request_url_end + strlen("HTTP/1.0\r\n") + 1;

	// if loop that will parse through the url and leave if error happens
    if (parse_url_method(http_request_url, host_name, path_name, &server_port_num) < 0)
	{

        close(connection_file_directory);
        free(http_request);
		pthread_exit(NULL);
    }

	pthread_mutex_lock(&lock_the_function);

	// if loop that will make the server listen to client requests
	
    server_file_directory = open_clientfd(host_name, server_port_num);
    if (server_file_directory < 0)
	{

        free(http_request);
		pthread_mutex_unlock(&lock_the_function);
		pthread_exit(NULL);
    }
	pthread_mutex_unlock(&lock_the_function);
	pthread_mutex_lock(&lock_the_function);

	// rio written that will extract various words from url
    writing_failing_method(server_file_directory, "GET /", strlen("GET /"));
    writing_failing_method(server_file_directory, path_name, strlen(path_name));
    writing_failing_method(server_file_directory, " HTTP/1.0\r\n", strlen(" HTTP/1.0\r\n"));
    writing_failing_method(server_file_directory, rest_of_http_request, strlen(rest_of_http_request));
	pthread_mutex_unlock(&lock_the_function);
	Rio_readinitb(&rio_object, server_file_directory);
    http_response_length = 0;

	// while loop that will take in server requests and buffer request
	while( (b = read_failing_method(server_file_directory, buffer_object, MAXLINE)) > 0 )
	{
        http_response_length += b;
        writing_failing_method(connection_file_directory, buffer_object, b);
        bzero(buffer_object, MAXLINE);
    }

	pthread_mutex_lock(&lock_the_file);

	// method to write to the file
	format_log_entry(text_log_entry_object, clientaddrPtr, http_request_url, http_response_length);
    fprintf(text_log, "%s %d\n", text_log_entry_object, http_response_length);
    fflush(text_log);

	// closing and locking all file directorys, requests and leaving the thread
	pthread_mutex_unlock(&lock_the_file);
    close(connection_file_directory);
    close(server_file_directory);
    free(http_request);

	pthread_exit(NULL);
}





/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{

	struct sockaddr_in client_address;

	// lock the file and function
	pthread_mutex_init(&lock_the_function,NULL);
	pthread_mutex_init(&lock_the_file,NULL);

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
		exit(0);
	}

	signal(SIGPIPE, SIG_IGN);

	int listen_file_dir = Open_listenfd(atoi(argv[1]));

	// open the text log
	text_log = Fopen("PLog.txt", "a");

	// to keep on listening continously
	while (1) {

		int client_length = sizeof(client_address);
		int connection_file_dir = Accept(listen_file_dir, (SA *)&client_address, &client_length);

		// Assemble argument struct for the thread
		struct argument_structure thread_arguments;
		struct argument_structure *thread_arguments_pointer;
		thread_arguments.client_address_pointer = &client_address;
		thread_arguments.connection_fd = connection_file_dir;
		thread_arguments_pointer = &thread_arguments;

		pthread_t thread_id;
		int temp_value;
		temp_value = pthread_create(&thread_id,NULL,thread_process_method,(void *)thread_arguments_pointer);
		if(temp_value != 0)
		{
			continue;
		}

	}

	// exit line
	exit(0);
}


/*
 * read_failing_method - on read failing print a message
 */
ssize_t read_failing_method(int filedir, void *pointer, size_t numofbytes)
{
    ssize_t a;

    if ((a = rio_readn(filedir, pointer, numofbytes)) < 0) {
	printf("Warning: reading failed\n");
	return 0;
    }
    return a;
}

/*
 * reading_failing_line_method - on read faling print a message
 */
ssize_t read_failing_line_method(rio_t *readpont, void *userbuffer, size_t maxlength)
{
    ssize_t b;

    if ((b = rio_readlineb(readpont, userbuffer, maxlength)) < 0) {
	printf("Warning: reading line failed\n");
	return 0;
    }
    return b;
}

/*
 * writing_failing_method - writing failing
 */
void writing_failing_method(int filed, void *userbuffer, size_t c)
{
    if (rio_writen(filed, userbuffer, c) != c) {
	printf("Warning: writing failed.\n");
    }
}

/*
 * parse_uri - URI parser
 *
 * This method basically takes in the HTTP GET request, host name , path name, port
 * all extracted will return the int value of the url. The memory has already been allocated
 */
int parse_url_method(char *url, char *host_name, char *path_name, int *port_number)
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
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
		      char *uri, int size)
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
    sprintf(logstring, "%s: %d.%d.%d.%d %s", time_str, a, b, c, d, uri);
}
