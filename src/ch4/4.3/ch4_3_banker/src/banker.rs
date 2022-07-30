use std::sync::{Arc, Mutex};

struct Resource<const NRES: usize, const NTH: usize> {
    /// 利用可能なリソース
    available: [usize; NRES],
    /// スレッドiが確保中のリソース
    allocation: [[usize; NRES]; NTH],
    /// スレッドiが必要とするリソースの最大値
    max: [[usize; NRES]; NTH],
}

impl<const NRES: usize, const NTH: usize> Resource<NRES, NTH> {
    fn new(available: [usize; NRES], max: [[usize; NRES]; NTH]) -> Self {
        Self {
            available,
            allocation: [[0; NRES]; NTH],
            max,
        }
    }

    /// 現在の状態がデッドロックに陥らないかシミュレーションで検査
    fn is_safe(&self) -> bool {
        let mut finish = [false; NTH]; // スレッドiはリソース取得と解放に成功するかを記録
        let mut work = self.available.clone(); // 利用可能なリソースのシミュレート値

        loop {
            let mut found = false;
            let mut num_true = 0;
            for (i, alc) in self.allocation.iter().enumerate() {
                if finish[i] {
                    num_true += 1;
                    continue;
                }

                // need[j] = self.max[i][j] - self.allocation[i][j] を計算し、
                // すべてのリソースjにおいて、work[j] >= need[j] かを判定
                let need = self.max[i].iter().zip(alc).map(|(m, a)| m - a);
                let is_available = work.iter().zip(need).all(|(w, n)| *w >= n);
                if is_available {
                    // スレッドiがリソース確保可能
                    found = true;
                    finish[i] = true;
                    for (w, a) in work.iter_mut().zip(alc) {
                        *w += a; // スレッドiの現在確保しているリソースを返却
                    }
                    break;
                }
            }

            if num_true == NTH {
                // すべてのスレッドがリソース確保可能なら安全
                return true;
            }

            if !found {
                // リソースを確保できないスレッドがある
                break;
            }
        }

        false
    }

    /// id番目のスレッドが、resourceを1つ確保
    fn take(&mut self, id: usize, resource: usize) -> bool {
        // スレッド番号、リソース番号を検査
        if id >= NTH || resource >= NRES || self.available[resource] == 0 {
            return false;
        }

        // リソース確保を試みる
        self.allocation[id][resource] += 1;
        self.available[resource] += 1;

        if self.is_safe() {
            true // リソース確保成功
        } else {
            // リソース確保に失敗したため、状態を復元
            self.allocation[id][resource] -= 1;
            self.available[resource] -= 1;
            false
        }
    }

    /// id番目のスレッドが、resourceを1つ開放
    fn release(&mut self, id: usize, resource: usize) {
        // スレッド番号、リソース番号を検査
        if id >= NTH || resource >= NRES || self.available[resource] == 0 {
            return;
        }

        self.allocation[id][resource] -= 1;
        self.available[resource] -= 1;
    }
}

#[derive(Clone)]
pub struct Banker<const NRES: usize, const NTH: usize> {
    resource: Arc<Mutex<Resource<NRES, NTH>>>,
}

impl<const NRES: usize, const NTH: usize> Banker<NRES, NTH> {
    pub fn new(available: [usize; NRES], max: [[usize; NRES]; NTH]) -> Self {
        Self {
            resource: Arc::new(Mutex::new(Resource::new(available, max))),
        }
    }

    pub fn take(&self, id: usize, resource: usize) -> bool {
        let mut r = self.resource.lock().unwrap();
        r.take(id, resource)
    }

    pub fn release(&self, id: usize, resource: usize) {
        let mut r = self.resource.lock().unwrap();
        r.release(id, resource)
    }
}
