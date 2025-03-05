CREATE TABLE IF NOT EXISTS ReceiverConfig (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    mode TEXT NOT NULL,
    port TEXT NOT NULL,
    baudRate INTEGER NOT NULL
);

INSERT INTO ReceiverConfig (mode, port, baudRate)
VALUES ('standalone', '/dev/tty.usbmodem14101', 9600);
