# Sockets-full-duplex-chat

Instrucciones:
	1- Compilar y ejecutar desde computador A `g++ -o chat chat.cpp -std=c++11 -lfltk -pthread`.
	2- Compilar desde computador B `g++ -o chat chat.cpp -std=c++11 -lfltk -pthread` y al ejecutar `chat` agregar parámetro ip de A.

Todas las funciones llamadas desde hilos se declararon en fun.c.
Diseñado para ser ejecutado en dos computadores distintos, se define un puerto(de no ser así leer instrucciones en código).
Desde el main de chat.cpp se invocan las funciones `main_a` y `main_b` encargadas de crear los 4 hilos cliente y servidor de A y B.
Ambos hilos servidor (de A y B) usan la misma función t_server.
Los hilos cliente usan client_a y client_b, cuya diferencia es que uno ya contiene información sobre la conexión y el otro la establece para crearla.
La función hand de chat.cpp define un handler para cuando el otro envíe un mesaje este sea mostrado en pantalla inmediatamente.