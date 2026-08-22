package main

import (
	"fmt"
	"practic/http"
	"practic/todo"
)

func main() {
	todoList := todo.NewList()
	httpHandlers := http.NewHTTPHandler(todoList)
	httpServer := http.NewHTTPServer(httpHandlers)
	if err := httpServer.StartServer(); err != nil {
		fmt.Println("fail; to start http server")
	}
}
