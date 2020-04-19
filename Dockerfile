FROM centos
RUN yum update -y && yum upgrade -y
RUN yum install gcc gdb -y
RUN mkdir Lab_3
COPY arrays.s /Lab_3
COPY prog.c /Lab_3
