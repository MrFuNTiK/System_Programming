#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "string.h"
#include <sstream>


int main()
{
	unsigned int length = 10, ammount, low, high, last;
	pid_t pid = getpid();
	int rv;
	std::cout << "Enter low and high bound\n";
	std::cin >> low >> high;
	std::cout << "PARENT:\nMy pid: " << pid << "\n";


	ammount = (high - low) / length;
	last = (high - low) % length;
	if (last != 0)
		ammount++;
	
	for (unsigned int i = 0; i < ammount; i++)
	{
		unsigned int first = i * length + low;
		unsigned int second = i * length + low + length - 1;
		std::cout << "\nPARENT:\n" << "Trying to start child number " << i+1 << "\n";
		switch (pid = fork())
		{
			case -1:
				perror("fork");
				exit(1);
			case 0:
				//child_pid = getpid();	
				std::cout << "Success!\n";
				//std::cout << first_p << " " << second_p << "\n";
				execl("FindPlain", "FindPlain", std::to_string(first).c_str(), std::to_string(second).c_str(), (char*)NULL);
				exit(rv);
			default:
				wait(&rv);
				std::cout << "PARENT:\n";
				std::cout << "Child number " << i+1 << " terminated wth status " << rv << "\n";
		}
	}
    return 0;
}