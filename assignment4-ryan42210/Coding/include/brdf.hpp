#pragma once
#include <vector>
#include "Eigen/Dense"
#include "interaction.hpp"
#include "utils.hpp"
#include "constant.hpp"


/**
 * Base class of BRDFs
 */
class BRDF
{
public:
    BRDF();
    
    /* Evaluate the BRDF, namely, return the BRDF value at the given interation */
    virtual Eigen::Vector3f eval(const Interaction& interact) = 0;
    /* Compute the PDF of the given BRDF sample at the specified interaction
     * You may need to use the Interaction.wi and Interaction.wo
     */
    virtual float samplePdf(const Interaction& interact) = 0;
    /* Sample a direction according to the BRDF
     * Store the sampled direction in the given interaction
     * Also, the PDF of this sample should be returned
     */
    virtual float sample(Interaction& interact) = 0;
    /* Return if the BRDF is delta (specular or transmission) */
    virtual bool isDelta() const = 0;

};


/**
 * Ideal diffusion BRDFs
 */

class IdealDiffusion : public BRDF
{
protected:
    Eigen::Vector3f reflectivity;
    
public:
    IdealDiffusion(Eigen::Vector3f diff);
    virtual Eigen::Vector3f eval(const Interaction& interact) override;
    virtual float samplePdf(const Interaction& interact) override;
    virtual float sample(Interaction& interact) override;
    virtual bool isDelta() const override;
};


/**
 * Ideal specular BRDFs
 */

class IdealSpecular : public BRDF
{
protected:
    Eigen::Vector3f reflectivity;

public:
    IdealSpecular(Eigen::Vector3f spec);
    virtual Eigen::Vector3f eval(const Interaction& interact) override;
    virtual float samplePdf(const Interaction& interact) override;
    virtual float sample(Interaction& interact) override;
    virtual bool isDelta() const override;
};


/**
 * Ideal transmission BRDFs
 */

#define IOR_DIAMOND 2.417f
#define IOR_AMBER   1.550f
#define IOR_WATER   1.333f
#define IOR_GLASS   1.520f
#define IOR_VACUUM  1.000f

class IdealTransmission : public BRDF
{
protected:
    float ior;
    Eigen::Vector3f reflectivity;

public:
    IdealTransmission(Eigen::Vector3f reflect, float idx_refract = 1.0f);
    virtual Eigen::Vector3f eval(const Interaction& interact) override;
    virtual float samplePdf(const Interaction& interact) override;
    virtual float sample(Interaction& interact) override;
    virtual bool isDelta() const override;
};