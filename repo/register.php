<?php
  //Результаты операции
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог вставить новое имя в базу
  // 4 - имя уже есть в базе и пароль не совпал
  if( $_POST["author"] && $_POST["password"] && $_POST["email"] ) {
    $link = mysqli_connect("localhost", "host1328365_salicad", "X6uX7T0a", "host1328365_salicad");
    if( $link == false ) {
      $array = array(
        "result" => 2
        );
      }
    else {
      mysqli_set_charset($link, "utf8");
      //Проверяем наличие уже зарегистрированного пользователя
      $result = mysqli_query( $link, 'SELECT * FROM users WHERE author = "'.$_POST["author"].'"');
      if( mysqli_num_rows($result) == 0 ) {
        //Пользователь еще не зарегистрирован, регистрируем
        $sql = 'INSERT INTO users SET author = "' .$_POST["author"]. '", password = "' .$_POST["password"]. '", email = "' .$_POST["email"]. '"';
        $result = mysqli_query($link, $sql );
        if( $result == true ) {
          //Успешно зарегистрирован
          $array = array(
            "result" => 0,
            "email" => $_POST["email"]
            );
          }
        else {
          //Не смог вставить имя в базу данных
          $array = array(
            "result" => 3
            );
          }
        }
      else {
        //Пользователь с данным именем уже есть в базе, проверим совпадает ли пароль
        $row = mysqli_fetch_array($result);
        if( $row["password"] == $_POST["password"] ) {
          //Пароль совпадает, извлекаем email
          $array = array(
            "result" => 0,
            "email" => $row["email"]
            );
          }
        else {
          //Пароль не совпадает - дублирование имени
          $array = array(
            "result" => 4
            );
          }
        }
      }
    }
  else {
    //Проблема со входными данными
    $array = array(
      "result" => 1
      );
    }
  header('Content-Type: application/json');
  echo json_encode($array);
  exit();
  
?>