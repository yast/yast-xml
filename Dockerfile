FROM yastdevel/cpp:sle12-sp5

RUN zypper --non-interactive in --no-recommends \
  libxml2-devel
COPY . /usr/src/app
