FROM gcc
COPY . /GoodbyeWorld
WORKDIR /GoodbyeWorld/
RUN gcc -o GoodbyeWorld main.c
CMD ["./GoodbyeWorld"]