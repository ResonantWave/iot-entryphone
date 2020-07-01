# iot-entryphone
Converting an old manual entryphone on a budget 

### What is this?
Just a quick afternoon project that enables any old entryphone into an IoT one, through a normal URL. This way no app or previous setup is needed, only the link itself.

The system consists of a NodeMCU with a relay directly connected to the entryphone open button. Then a server with a MySQL instance, allowing a quick and dirty access control, with unique and revocable keys.

### Hardware
You will need a NodeMCU or similar device. The firmware is available [here](entryphone-node/entryphone-node.ino). Don't forget to fill in your WiFi details!
There's also a code block that, when uncommented, will assign a static IP address to the NodeMCU (this is probably what you want)

### Access Control
An access link is like: `https://example.com/doorAccess.php?key=c848d501fe9b677754d37e8a4f0efb31`
The main script is [doorAccess.php](doorAccess.php). It will connect to the database and check if the given key exists, has remaining accesses and has not expired. If everything is ok, it will send a request to the NodeMCU and the entryphone will open.

Each key is just a random string, but any text will work (it justs need to be unique). If you want to give infinite access to specific keys, when creating the record set the allowedAccesses to -1. Also, if you only want to give access until certain date and time, set the maximumDate appropiately. There's also a details field to save any important note or data about that key.

By default, when a key is used an access is recorded on the AccessLog table. Date, key and origin IP address are saved. This can be easily disabled by deleting the corresponding lines in [doorAccess.php](doorAccess.php)

### SQL table structure

You will need a MySQL/MariaDB server. Here's the main table structure:
```sql
CREATE TABLE `AccessCredentials` (
  `accessKey` varchar(50) NOT NULL,
  `allowedAccesses` int(6) NOT NULL DEFAULT 1,
  `maximumDate` int(8) NOT NULL DEFAULT current_timestamp(),
  `details` varchar(100) DEFAULT '',
  PRIMARY KEY (`accessKey`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

And here's the access log table structure:
```sql
CREATE TABLE `AccessLog` (
  `accessDate` int(8) NOT NULL DEFAULT current_timestamp(),
  `accessKey` varchar(50) NOT NULL,
  `ip` varchar(16) NOT NULL
);
```

### Creating keys
I made this quick script to ease the creating of keys. Use it as a reference, and please create something better than I did 🤦‍♂️
[keyCreateDemo.php](keyCreateDemo.php)

### Notes
To quickly open the entryphone from my phone, I created on my home screen a shortcut to the access URL.

### Contributors
 *  [@ResonantWave](https://github.com/ResonantWave)

### Contributing
 *  The code is licensed under the [AGPL v3.0](LICENSE)
 *  Feel free to contribute to the code
