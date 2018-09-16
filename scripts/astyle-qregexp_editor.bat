@echo off 
:: apply kdelibs coding style to all c, cpp and header files in and below the current directory 
::
:: the coding style is defined in http://techbase.kde.org/Policies/Kdelibs_Coding_Style 
::
:: requirements: installed astyle => 1.24

setlocal EnableDelayedExpansion

SET ASTYLE_OPTS=--indent=spaces=4 --brackets=stroustrup --add-brackets ^
                 --indent-labels --pad-header --pad-oper --unpad-paren ^
                 --keep-one-line-blocks --convert-tabs --indent-preprocessor ^
                 --indent-namespaces --align-pointer=name

FOR /R %%g IN (*.c *.cpp *.h) DO (
    astyle %ASTYLE_OPTS%  %%g
)

endlocal
