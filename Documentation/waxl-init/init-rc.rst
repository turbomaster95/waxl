==================================
waxl-init Configuration (init.rc)
==================================

Overview
========

The ``waxl-init`` process uses an Android-style initialization script (typically located at ``/etc/init.rc``) to configure virtual file system state, manage system properties, fire lifecycle triggers, and supervise daemons.

Syntax Rules
============

* **Comments:** Any text following a ``#`` character on a line is treated as a comment and ignored.
* **Whitespace:** Blank lines and leading/trailing whitespace are ignored. Arguments are whitespace-delimited.
* **Block Structure:** The file is parsed sequentially into two main section types: **Actions** (``on``) and **Services** (``service``).

Actions & Triggers
==================

Actions are blocks of commands that execute sequentially when a specific event trigger is fired.

Syntax::

    on <trigger_name>
        <command_1> [args...]
        <command_2> [args...]

Built-in Boot Triggers
-----------------------

During system startup, ``waxl-init`` automatically fires four built-in lifecycle events in exact sequence:

1. ``early-init`` — Fired immediately after initial virtual filesystems (proc, sysfs, devtmpfs, cgroups, etc.) are mounted and signal handlers are attached.
2. ``init`` — Primary setup phase for directory structure and core system properties.
3. ``boot`` — Default initialization stage. All non-disabled background services are spawned immediately after this stage completes.
4. ``post-boot`` — Final startup trigger fired after all initial service processes have been fork/exec'd.

Property Triggers
-----------------

When a property is set using ``setprop``, an automatic property event is dispatched in the format ``property:<key>=<value>``.

Example::

    on property:sys.boot_completed=1
        write /dev/kmsg "Boot process finished."

Commands
========

The following commands are available inside ``on`` action blocks:

Filesystem Operations
---------------------

``mkdir <path> [mode]``
    Creates a directory at ``path``. If optional octal ``mode`` is omitted, defaults to ``0755``.

``write <path> <content>``
    Opens ``path`` (truncating existing contents) and writes string ``content``.

``chmod <octal_mode> <path>``
    Changes file mode bits of ``path`` using octal representation (e.g., ``0644``).

``chown <uid> <gid> <path>``
    Changes numeric ownership UID and GID for ``path``.

``symlink <target> <linkpath>``
    Creates a symbolic link at ``linkpath`` pointing to ``target``.

``rm <path>``
    Unlinks (deletes) the specified file path.

Service Control
---------------

``start <service_name>``
    Forks and executes the specified service.

``stop <service_name>``
    Sends a ``SIGTERM`` signal to the service process.

``restart <service_name>``
    Stops the specified service and re-spawns it immediately.

System & Execution Control
--------------------------

``setprop <key> <value>``
    Updates or inserts system property ``key`` with ``value`` and dispatches its ``property:key=value`` trigger.

``trigger <event_name>``
    Synchronously fires all action blocks associated with ``event_name``.

``import <filepath>``
    Parses an additional `.rc` file at ``filepath``.

``exec <path> [args...]``
    Forks and synchronously executes a executable binary, blocking ``waxl-init`` execution until the child exits.

Services
========

Services represent daemons or oneshot tasks supervised by ``waxl-init``. If a non-oneshot service exits unexpectedly, ``waxl-init`` will automatically restart it.

Syntax::

    service <service_name> <binary_path> [args...]
        <option_1>
        <option_2>

Service Options
---------------

``oneshot``
    Do not restart the service when it exits.

``critical``
    Marks the service as system-critical.

``disabled``
    Prevents the service from launching automatically during the default boot sequence. Must be started explicitly via ``start <service_name>``.

``user <uid>``
    Sets the target process UID using ``setuid()`` before calling ``execv()``.

``group <gid>``
    Sets the target process GID using ``setgid()`` before calling ``execv()``.

``setenv <key> <value>``
    Injects an environment variable into the child process environment via ``putenv()``.

Complete Example
================

.. code-block:: initrc

    # /etc/init.rc for waxl-init

    on early-init
        mkdir /dev/pts 0755
        mkdir /sys/fs/cgroup 0755

    on init
        setprop sys.booting 1
        import /etc/init.custom.rc

    on boot
        chmod 0666 /dev/null
        write /proc/sys/kernel/printk "4"
        start console

    on post-boot
        setprop sys.boot_completed 1

    on property:sys.boot_completed=1
        exec /bin/echo "waxl system boot complete"

    service console /bin/sh
        user 0
        group 0
        setenv TERM linux

    service syslogd /sbin/syslogd -n
        disabled
        oneshot
