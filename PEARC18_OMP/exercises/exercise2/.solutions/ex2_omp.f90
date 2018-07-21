      program ex2_omp

!***  Stencil update
!***  This is the serial version
!***    Employs a temporary array (tmp) inside loop
!***    Employs a reduction operation on variable sum
!***    In this code there are read-after-write and write-after-read dependencies

!***  The algorithm that presets the input arrays and also the format for the sum
!***    are carefully selected to hide round-off discrepancies in the output

!***  All variables and unnamed constants are in double precision

!***  Compile with: ifort -xhost -qopenmp ex2_omp.f90

!***  Execution time is of the order of seconds
!***  Memory requirement is about 20.1 GB

!***  The kernel has a write-after-read (WAR) dependency

        use omp_lib

        integer                              :: n = 30000
        real(8), dimension(:,:), allocatable :: x, y, tmp
        real(8)                              :: sum

        write (0,*) 'Allocate arrays'
        allocate (x(n,n), y(n,n), tmp(n,n))

!***  The input arrays are carefull filled to minimize round-off errors
        write (0,*) 'Preset input arrays (x and y)'
        do j=1, n
          do i=1, n
            x(i,j) = real(modulo((i+j), 3)) - 0.9999_8
            y(i,j) = x(i,j) + 0.0001
          enddo
        enddo

!$      write (0,'(a,i4)') ' Number of threads :: ', omp_get_max_threads()

        write (0,*) 'Start timer and calculation'
        call system_clock(ic1, ir)

        sum = 0.

!$omp parallel do default(none) shared(n, x, y, tmp) private(i, j)
        do j=2, n-1
          do i=2, n-1
            tmp(i,j) = 0.167_8 * (x(i,j) + x(i-1,j) + x(i+1,j) + x(i,j-1) + x(i,j+1) + &
                                  y(i+1,j))
          enddo
        enddo

!$omp parallel do default(none) shared(n, y, tmp) private(i, j) reduction (+: sum)
        do j=2, n-1
          do i=2, n-1
            y(i,j)   = tmp(i,j)
            sum      = sum + tmp(i,j)
          enddo
        enddo

        call system_clock(ic2, ir)
        write (0,*) 'Stop timer after calculation'
        write (0,*)
        write (0,'(a,f12.5,a)') 'Time to completion :: ', (real(ic2) - real(ic1)) / real(ir), ' seconds'

!***  The output format with 2 digits after the decimal point is carefully selected to hide round-off errors
        write (0,'(a,f12.2)')   'The total sum is   :: ', sum

      end program ex2_omp
