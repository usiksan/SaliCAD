<?php
  //Результаты операции
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог найти объект в базе
  // 4 - имя или пароль не совпал
  if( $_POST["author"] && $_POST["password"] && $_POST["index"] ) {
    $link = mysqli_connect("localhost", "host1328365_salicad", "X6uX7T0a", "host1328365_salicad");
    if( $link == false ) {
      $array = array(
        "result" => 2
        );
      }
    else {
      mysqli_set_charset($link, "utf8");
      //Преобразовать строки в безопасные
      $author = mysqli_real_escape_string( $link, $_POST["author"]);
      $password = mysqli_real_escape_string( $link, $_POST["password"]);
      //Проверяем наличие уже зарегистрированного пользователя
      $result = mysqli_query( $link, 'SELECT * FROM users WHERE author = "'.$author.'" AND password = "'.$password.'"');
      if( mysqli_num_rows($result) == 0 ) {
        //Пользователь не зарегистрирован или пароль не совпал
        $array = array(
          "result" => 4
          );
        }
      else {
        //Пользователь с данным именем и паролем обнаружен
        //Проверить наличие в базе запрошенного объекта
        $result = mysqli_query( $link, 'SELECT object FROM objects WHERE gindex = '.$_POST["index"] );
        if( mysqli_num_rows($result) == 0 ) {
          //Запрошенного объекта нету в базе
          $array = array(
            "result" => 3,
            "index"  => $_POST["index"]
            );
          }
        else {
          //Объект найден, вернуть его
          $row = mysqli_fetch_array($result);
          $array = array(
            "result" => 0,
            "index"  => $_POST["index"],
            "object" => $row["object"]
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