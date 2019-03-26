FROM yastdevel/cpp:sle15-sp1
RUN zypper --gpg-auto-import-keys --non-interactive in --no-recommends \
  libxml2-devel
COPY . /usr/src/app

