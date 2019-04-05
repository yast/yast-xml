FROM registry.opensuse.org/yast/head/containers/yast-cpp:latest
RUN zypper --gpg-auto-import-keys --non-interactive in --no-recommends \
  libxml2-devel
COPY . /usr/src/app

