/******************************************************************************
 * @section DESCRIPTION
 *
 * This subroutine returns a parameter to multiply with Ksat to modify it for
 * the effects of temperature on the viscosity and density of water.  It is
 * assumed that the given Ksat value was measured at 20C (68F).
 *
 * @section LICENSE
 *
 * The Variable Infiltration Capacity (VIC) macroscale hydrological model
 * Copyright (C) 2014 The Land Surface Hydrology Group, Department of Civil
 * and Environmental Engineering, University of Washington.
 *
 * The VIC model is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *****************************************************************************/

#include <vic_def.h>
#include <vic_run.h>

/******************************************************************************
 * @brief    This routine performs a linear interpolation
 *****************************************************************************/
double
linear_interp(double x,
              double lx,
              double ux,
              double ly,
              double uy)
{
    return((x - lx) / (ux - lx) * (uy - ly) + ly);
}

/******************************************************************************
 * @brief    This subroutine interpolates the soil temperature at a given depth
 *
 * @note     This assumes that the temperature decays exponentially with depth
 *           from a surface temperature of "ly" to an asymptotic limit of "uy".
 *           "ux" here is the "damping" depth, at which difference between
 *           temperature at that depth and the asymptotic deep temperature is
 *           1/e of the diffence at the surface.
 *****************************************************************************/
double
exp_interp(double x,
           double lx,
           double ux,
           double ly,
           double uy)
{
    return(uy + (ly - uy) * exp(-(x - lx) / ux));
}

/******************************************************************************
 * @brief    This subroutine returns a parameter to multiply with Ksat to
 *           modify it for the effects of temperature on the viscosity and
 *           density of water.  It is assumed that the given Ksat value was
 *           measured at 20C (68F).
 *
 * @note     Viscosity and density taken from Linsley, "Hydrology for Engineers",
             A-10

            Temp  Rho     Mu     Factor
            C     kg/m^3  mPa-s
            0     999.84  1.79   0.560
            5     999.96  1.52   0.659
            10    999.70  1.31   0.770
            15    999.10  1.14   0.878
            20    998.21  1.00   1.00
            25    997.05  0.890  1.12
            30    995.65  0.798  1.25
            35    994.04  0.719  1.39
            40    992.22  0.653  1.52
 *****************************************************************************/
double
modify_Ksat(double Temp)
{
    extern option_struct options;

    double               Factor;

    /** formula generated by multiple regression against kinematic
        viscosity data from the Handbook of Chemistry and Physics **/
    if (options.FROZEN_SOIL) {
        Factor = 0.003557 / (0.006534 - 0.0002282 * Temp + 4.794e-6 * (Temp) *
                             (Temp) - 4.143e-8 * (Temp) * (Temp) * (Temp));
    }
    else {
        Factor = 1.;
    }

    if (Factor > 2.) {
        Factor = 2.;
    }

    return (1.0);
}