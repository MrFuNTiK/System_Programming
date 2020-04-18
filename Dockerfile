FROM centos
RUN yum update -y && yum upgrade -y
RUN yum install gcc gdb -y
RUN mkdir Lab_2
COPY array.c /Lab_2
COPY prog.s /Lab_2
COPY hello.s /Lab_2