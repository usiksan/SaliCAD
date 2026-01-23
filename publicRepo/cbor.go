package main

import (
    "fmt"
    "log"
    "github.com/fxamacker/cbor/v2"
)

func test() {
    // 1. Создаем динамическую мапу как в Qt
    data := map[interface{}]interface{}{
        int64(1):   int64(123),        // ID как целое число
        int64(2):   []byte{0x01, 0x02, 0x03}, // Данные
        "version":  "1.0",             // Можно и строковые ключи!
        int64(100): true,              // Дополнительное поле
    }

    // 2. Сериализуем в CBOR
    cborData, err := cbor.Marshal(data)
    if err != nil {
        log.Fatal(err)
    }

    fmt.Printf("CBOR размер: %d байт\n", len(cborData))
    fmt.Printf("Hex: %x\n", cborData)

    // 3. Десериализуем обратно в динамическую структуру
    var decoded map[interface{}]interface{}
    err = cbor.Unmarshal(cborData, &decoded)
    if err != nil {
        log.Fatal(err)
    }

    // 4. Работаем с данными (типобезопасно)
    fmt.Println("\nДесериализованные данные:")
    for key, value := range decoded {
        fmt.Printf("Ключ: %v (%T), Значение: %v (%T)\n",
            key, key, value, value)
    }

    // 5. Извлекаем конкретные значения
    if id, ok := decoded[int64(1)].(int64); ok {
        fmt.Printf("\nID: %d\n", id)
    }

    if dataBytes, ok := decoded[int64(2)].([]byte); ok {
        fmt.Printf("Данные: %v\n", dataBytes)
    }

    // Новое поле (может отсутствовать в старых версиях)
    if extra, ok := decoded[int64(100)]; ok {
        fmt.Printf("Доп. поле: %v\n", extra)
    }
}
