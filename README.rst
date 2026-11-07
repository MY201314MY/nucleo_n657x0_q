.. zephyr:code-sample:: hello_world
   :name: Hello World

   Print "Hello World" to the console.

Overview
********

A simple sample that can be used with any :ref:`supported board <boards>` and
prints "Hello World" to the console.

Building and Running
********************

This application can be built and executed on QEMU as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :host-os: unix
   :board: qemu_x86
   :goals: run
   :compact:

To build for another board, change "qemu_x86" above to that board's name.

Sample Output
=============

.. code-block:: console

    Hello World! x86

Exit QEMU by pressing :kbd:`CTRL+A` :kbd:`x`.

Hardware Flash Control
**********************

BOOT0 remains low (0) throughout the entire procedure. PA5 is connected
to BOOT0, and the script does not drive PA5. PA6 controls BOOT1,
and PA7 controls target power (1 for on, 0 for off).
The target has been initially be powered on.

Run ``bash ext.sh`` to perform the following sequence:

#. Set BOOT1 high (PA6=1), keeping BOOT0 low.
#. Reset the target using
   ``STM32_Programmer_CLI -c port=SWD mode=UR -hardRst``.
#. Run ``west flash`` and wait for successful completion.
#. Set BOOT1 low (PA6=0), keeping BOOT0 low.
#. Power off the target (PA7=0), wait 0.5 seconds, then power it on
   (PA7=1) with BOOT0=0 and BOOT1=0.

``west`` and ``STM32_Programmer_CLI`` must be available on ``PATH``.

The control board is selected using the shell wildcard pattern
``/dev/serial/by-id/*Zephyr_USB_CDC_ACM*-if00``, not its changing
``/dev/ttyACM*`` number. The by-id name normally stays the same across
reconnections of the same board. Connect only one matching control board.
The script stops if no device or multiple devices match, a serial write
fails, resetting fails, or flashing fails.

Debugging
*********

Run ``sh debug.sh`` after flashing the application. The script loads
MCUboot, runs to ``do_boot``, adds the application symbols, and stops at
the application's ``main``. Enter ``c`` in GDB to run the application.
GDB startup commands are kept in ``debug.sh`` itself; no temporary files
or separate wrapper scripts are needed.
