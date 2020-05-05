FROM centos
RUN yum update -y && yum upgrade -y
RUN yum install gcc gdb -y
RUN mkdir Lab_4
COPY prog.cpp /Lab_4
COPY FindPlain.cpp /Lab_4
