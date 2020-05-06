FROM centos
RUN yum update -y && yum upgrade -y
RUN yum groupinstall -y 'Development Tools'
RUN mkdir Lab_4
COPY prog.cpp /Lab_4
COPY FindPlain.cpp /Lab_4
