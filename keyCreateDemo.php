<?php
$DBH = new PDO("mysql:host=localhost;dbname=DoorAccess", "user", "pass");

$maxDate = time() + 24*60*60;
$accessKey = generateRandomString(30);
$allowedAcceses = 3;
$details = "A great detail about this key.";

$result = $DBH->prepare("INSERT INTO AccessCredentials (accessKey, allowedAccesses, maximumDate, details) VALUES (:accessKey, :allowedAcceses, :maximumDate, :details)");
$result->bindParam(':accessKey', $accessKey);
$result->bindParam(':allowedAcceses', $allowedAcceses);
$result->bindParam(':maximumDate', $maxDate);
$result->bindParam(':details', $details);
$result->execute();

function generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}
?>