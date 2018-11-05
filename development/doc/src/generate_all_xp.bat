del *.txt

rem First Pass

doxygen libx_core.cfg
copy Warnings.txt warnings_libx_core.txt /Y

doxygen libx_model.cfg
copy Warnings.txt warnings_libx_model.txt /Y

doxygen libx_viewer.cfg
copy Warnings.txt warnings_libx_viewer.txt /Y

;
; Second Pass
;
doxygen libx_core.cfg
doxygen libx_model.cfg
doxygen libx_viewer.cfg
