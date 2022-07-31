Rustの場合、再帰ロックの挙動は未定義となっている。  
このコードはデッドロックに陥る。  

```bash
cargo run --release
```
