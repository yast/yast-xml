#
# spec file for package yast2-xml
#
# Copyright (c) 2013 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#


Name:           yast2-xml
Version:        3.1.1
Release:        0

BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Source0:        %{name}-%{version}.tar.bz2

Group:	        System/YaST
License:        GPL-2.0+
BuildRequires:	gcc-c++ libtool doxygen yast2-core-devel libxml2-devel
BuildRequires:  yast2-devtools >= 3.0.6
Summary:	YaST2 - XML Agent
Requires:	yast2-core
Provides:	yast2-agent-xml yast2-agent-xml-devel yast2-lib-xml
Obsoletes:	yast2-agent-xml yast2-agent-xml-devel yast2-lib-xml

%description
The YaST2 XML agent

%prep
%setup -n %{name}-%{version}

%build
%yast_build

%install
%yast_install

rm -f $RPM_BUILD_ROOT/%{yast_plugindir}/libpy2ag_xml.la


%files
%defattr(-,root,root)
%{yast_plugindir}/libpy2ag_xml.so.*
%{yast_plugindir}/libpy2ag_xml.so
%{yast_scrconfdir}/xml.scr
%doc %{yast_docdir}
