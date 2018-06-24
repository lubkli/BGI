Summary:    BGI-compatible 2D graphics C library
Name:       xbgi
Version:    364
Release:    1
License:    Free software
Group:      Libraries
Source:     %{name}-%{version}.tar.gz
URL:        http://www.garret.ru/lang.html
Requires:   libx11-dev
Prefix:     %{_prefix}
BuildRoot:  %{_tmppath}/%{name}-%{version}-buildroot

%description
This package contains a Borland Graphics Interface (BGI) emulation
library for X11. This library strictly emulates most BGI functions,
making it possible to compile X11 versions of programs written for
Turbo/Borland C. RGB extensions and basic mouse support are also
implemented.

%prep
%setup -q -n %{name}-%{version}

%build
cd src
make

%install
rm -rf $RPM_BUILD_ROOT
cd src
mkdir -p $RPM_BUILD_ROOT/%{_libdir}
mkdir -p $RPM_BUILD_ROOT/%{_includedir}
cp libXbgi.a $RPM_BUILD_ROOT/%{_libdir}
cp graphics.h $RPM_BUILD_ROOT/%{_includedir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,root,755)
%doc Functions.txt Install.txt License.txt README TODO.txt
%doc Using.txt xbgi.spec src/test/
%attr(755,root,root) %{_libdir}/*
%attr(644,root,root) %{_includedir}/*

%changelog
* Wed Nov 27 2012 Guido Gonzato <guido.gonzato at gmail.com>
This is a generic rpm, buildable on Ubuntu
