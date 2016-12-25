FROM alpine:3.3

RUN apk update && apk upgrade && \
    apk add --no-cache bash git linux-headers build-base binutils

# Here we install GNU libc (aka glibc) and set C.UTF-8 locale as default.

ENV LANG=C.UTF-8

RUN git clone https://github.com/petr-tik/income_calc.git && \
    cd income_calc && \
    make income_calc_test2 && \
    make install

CMD /bin/sh

    
