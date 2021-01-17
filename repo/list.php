<?php
  //Результаты операции
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог вставить новое имя в базу
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
      $author = mysqli_real_escape_string( $link, $_POST["author"] );
      $password = mysqli_real_escape_string( $link, $_POST["password"] );
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
        //Cформировать список объектов
        $result = mysqli_query( $link, 'SELECT gindex, uid, time FROM objects WHERE gindex > '.$_POST["index"].' ORDER BY gindex' );
        $count = 0;
        $list = array();
        while( ($row = mysqli_fetch_array($result)) && $count < 100 ) {
          $list[ (string)$row["gindex"] ] = array(
            "uid"    => $row["uid"],
            "time"   => $row["time"]
            );
          $count = $count + 1;
          }
        //Список сформирован
        $array = array(
          "result" => 0,
          "list" => $list
          );
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