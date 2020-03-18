FROM centos
RUN yum update -y && yum upgrade -y
COPY script.sh .
