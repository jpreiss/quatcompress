# quatcompress
Compress a normalized quaternion into 32 bits.

On random quaternions, mean error is 0.08 degrees and worst-case error is 0.25 degrees.

Always returns a quaternion where the element of largest magnitude is positive.
This saves 1 bit, but it creates the possibility that quaternion elements will
flip signs when the underlying rotation only changes slightly.
Since `q` and `-q` represent the same rotation, code that uses quaternions
correctly should be able to handle sign flips without any problem.
