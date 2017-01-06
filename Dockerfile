FROM yastdevel/cpp-tw
RUN zypper --gpg-auto-import-keys --non-interactive in --no-recommends \
  libxml2-devel
COPY . /tmp/sources

