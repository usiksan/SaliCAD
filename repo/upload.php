<?php
  //Результаты операции
  // 0 - успешное завершение
  // 1 - проблема со входными данными
  // 2 - не может подключиться к базе данных
  // 3 - не смог вставить новый объект в базу
  // 4 - имя или пароль не совпал
  if( $_POST["author"] && $_POST["password"] && $_POST["uid"] && $_POST["time"] && $_POST["object"] ) {
    $link = mysqli_connect("localhost", "host1328365_salicad", "X6uX7T0a", "host1328365_salicad");
    if( $link == false ) {
      $array = array(
        "result" => 2
        );
      }
    else {
      mysqli_set_charset($link, "utf8");
      //Проверяем наличие уже зарегистрированного пользователя
      $result = mysqli_query( $link, 'SELECT * FROM users WHERE author = "'.$_POST["author"].'" AND password = "'.$_POST["password"].'"');
      if( mysqli_num_rows($result) == 0 ) {
        //Пользователь не зарегистрирован или пароль не совпал
        $array = array(
          "result" => 4
          );
        }
      else {
        //Пользователь с данным именем и паролем обнаружен
        //Проверить наличие в базе более нового объекта
        $result = mysqli_query( $link, 'SELECT gindex FROM objects WHERE time >= '.$_POST["time"].' AND uid = "'.$_POST["uid"].'"' );
        if( mysqli_num_rows($result) == 0 ) {
          //Переданный объект более новый чем в базе или такого объекта вообще еще нету, добавить новый объект
          $stmt = mysqli_prepare( $link, 'INSERT INTO objects (uid, time, object) VALUES( ?, ?, ? )' );
//          $result = mysqli_query( $link, 'INSERT INTO objects SET uid = "'.$_POST["uid"].'", time = '.$_POST["time"].',
//            object = "'.mysql_real_escape_string($_POST["object"]).'"' );

          if( $result == true ) {
            $array = array(
              "result" => 0
              );
            }
          else {
            //Ошибка при вставке
            $array = array(
              "result" => 3
              );
            }
          }
        else {
          //В базе более новый объект, нежели чем переданный
          //Ничего не делаем
          $array = array(
            "result" => 0
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
