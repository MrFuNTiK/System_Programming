FROM centos
RUN yum update -y && yum upgrade -y
RUN yum groupinstall 'Development Tools' -y
RUN mkdir Lab_6
COPY Lab6.cpp /Lab_6