Indicator for Ubuntu's default Unity shell, showing the current CPU/GPU power consumption by modern **intel** processors. The source code is in public domain.

.. image:: https://bitbucket.org/repo/MzL54B/images/3291845378-indicator-power-gadget-screenshot.png
   :alt: screenshot 

Building instructions
=====================

qwt
---

The gadget uses `Qwt - Qt Widgets for Technical Applications <http://qwt.sourceforge.net/>`_. A framework to draw plots of different kind. Static linking against version 6.x is used. First, you have to get the qwt, unpack it, and change the ``qwtconfig.pri`` file in it's root directory. Comment out the lines::

    #QWT_CONFIG           += QwtDll
    #QWT_CONFIG     += QwtWidgets
    #QWT_CONFIG     += QwtSvg
    #QWT_CONFIG     += QwtOpenGL
    #QWT_CONFIG     += QwtDesigner
    
Or you can download already patched qwt version `here. <https://bitbucket.org/baltic/power-gadget/downloads/qwt-6.1.1.tar.xz>`_ 
To build qwt just go into the derictory where you have unpacked it and run:: 

    qmake qwt.pro
    make
    sudo make instal
    
After which add the location where ``qwt.prf`` was installed to the qmake settings::

    qmake -set QMAKEFEATURES ${QWT_ROOT}/features

Where ``${QWT_ROOT}`` is normally something like ``/usr/local/qwt-6.1.1`` in the case of ubuntu.

See the `qwt install guide <http://qwt.sourceforge.net/qwtinstall.html>`_ for additional documentation.

building .deb package
---------------------
Just run the ``./make-deb`` script from the root of the source tree.