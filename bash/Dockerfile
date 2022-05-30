FROM java:8
VOLUME /tmp
MAINTAINER GMsure
ADD DataServer.jar Server.jar
EXPOSE 8500
ENTRYPOINT ["java","-jar","-Djava.net.preferIPv4Stack=true","Server.jar"]
