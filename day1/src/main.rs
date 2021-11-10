use std::env;
use std::vec::Vec;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;


fn main() {
    let mut numbers: Vec<i32> = Vec::new();
    let args: Vec<String> = env::args().collect();

    if let Ok(lines) = read_lines(&args[1])
    {
        for line in lines
        {
            if let Ok(num) = line
            {
                numbers.push(num.parse().unwrap());
            }
        }
    }
    numbers.sort();
    find_match(numbers, 2020);
}

fn find_match(v: Vec<i32>, t: i32) -> ()
{
    let l = v.len();
    for i in 0..(l - 2)
    {
        for j in (i + 1)..(l - 1)
        {
            let u = v[i] + v[j];
            if u == t
            {
                println!("x {}", v[i]);
                println!("y {}", v[j]);
                println!("Part 1 {}", v[i] * v[j]);
            }
            else if u < t
            {
                for k in (j + 1)..(l - 1)
                {
                    let w = u + v[k];
                    if w == t
                    {
                        println!("x {}", v[i]);
                        println!("y {}", v[j]);
                        println!("z {}", v[k]);
                        println!("Part 2 {}", v[i] * v[j] * v[k]);
                    }
                    else if w > t
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
}

// The output is wrapped in a Result to allow matching on errors
// Returns an Iterator to the Reader of the lines of the file.
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where P: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
