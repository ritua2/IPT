void calc_up(int x, int y, int Nx, int Ny, double u[Nx][Ny], double up[Nx][Ny]) {
  up[x][y] = u[x][y] + 0.01*(u[x-1][y]+u[x+1][y]-2*u[x][y]) + 0.01*(u[x][y-1]+u[x][y+1]-2*u[x][y]);
}
