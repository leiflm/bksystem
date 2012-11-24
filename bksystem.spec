Name:      bksystem
Summary:   An EFL based kiosk point of sale application.
Version:   0.1
Release:   1
License:   BSD
Packager:  Leif Middelschulte
Group:     Productivity/Office/Finance
Source:   %{name}-%version.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-build
BuildRequires: cmake eina-devel evas-devel ecore-devel elementary-devel sqlite3-devel

%description
BKSystem is a simple and lightweight kiosk-style point of sale application
based on the Enlightenment Foundation Libraries (http://www.enlightenment.org)

 
%prep
echo Building %{name}-%{version}-%{release}

%setup -q -n %{name}

%build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr .
make %{?_smp_mflags}

%install
make DESTDIR=%buildroot install

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)

# the binary files
%{_bindir}/%{name}

# the examplary database files
%_datadir/%{name}/example.sqlite
%_datadir/%{name}/empty.sqlite

# a script to create bills
%_datadir/%{name}/backup.sh
%_datadir/%{name}/bksystem.sh
%_datadir/%{name}/export_bill_basic.sh
%_datadir/%{name}/export_bill_complete_comb.sh
%_datadir/%{name}/export_bill_complete_sep.sh
%_datadir/%{name}/export_bill_html.sh
%_datadir/%{name}/insert_bill_reference_date_into_db.sh
%_datadir/%{name}/layout.css
%_datadir/%{name}/restore_db.sh
%_datadir/%{name}/sub_txt_tmpl

# desktop file
%_datadir/applications/bksystem.desktop

# bksystem icons
%_datadir/%{name}/images/accounts/locked.png

# docs, disabled for now
#%doc %_mandir/man1/kmymoney2.1.gz

%changelog
* Sat Nov 10 2012 - leif.middelschulte (at) gmail.com
- Initial packaging
