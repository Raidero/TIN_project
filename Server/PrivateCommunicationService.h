#ifndef PRIVATE_COMMUNICATION_SERVICE_H
#define PRIVATE_COMMUNICATION_SERVICE_H


typedef struct
{
    char* receiver;
} Header;

struct Message
{
	Header header;
    char* message;	/* overflow protection -- but only @ client? */
};




#endif // PRIVATE_COMMUNICATION_SERVICE_H
