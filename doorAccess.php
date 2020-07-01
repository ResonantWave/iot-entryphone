<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$DBH = new PDO("mysql:host=localhost;dbname=DoorAccess", "user", "password"); // you'll probably want to change these credentials

if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['key'])) {
	$result = $DBH->prepare("SELECT * FROM AccessCredentials WHERE accessKey = :accessKey AND allowedAccesses <> 0 AND maximumDate < CURRENT_TIMESTAMP");
	$result->bindParam(':accessKey', $_GET['key']);
	if($result->execute()) {
		if($result->rowCount() == 1) {
			$decreaseAccess = $DBH->prepare("UPDATE AccessCredentials SET allowedAccesses = allowedAccesses - 1 WHERE accessKey = :accessKey");
			$decreaseAccess->bindParam(':accessKey', $_GET['key']);
			if($decreaseAccess->execute()) {
				print file_get_contents("http://192.168.0.7/open?delay=2500"); // change this IP to the NodeMcu one

                // Optional: remove these 5 lines if you don't care about logging accesses
				$accessLog = $DBH->prepare("INSERT INTO AccessLog (accessKey, accessDate, ip) VALUES (:accessKey, :accessDate, :ip)");
				$accessLog->bindParam(':accessKey', $_GET['key']);
				$accessLog->bindParam(':accessDate', time());
				$accessLog->bindParam(':ip', $_SERVER['REMOTE_ADDR']);
				$accessLog->execute();
			}
		}
	}
}
?>