
export function fib(n) { // don't accept argument
    if (n <= 1){
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}