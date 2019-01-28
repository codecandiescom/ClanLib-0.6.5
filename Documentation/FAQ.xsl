<?xml version="1.0"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"/>

<xsl:template match="faq">
  <html>
	<head>
      <xsl:apply-templates select="version"/>
      <xsl:apply-templates select="title"/>
	</head>
    <body>
      <xsl:apply-templates select="p"/>
      <xsl:apply-templates select="section"/>
    </body>
  </html>
</xsl:template>

<xsl:template match="version">
  <version>
    <xsl:value-of select="."/>
  </version>
</xsl:template>

<xsl:template match="title">
  <title>
    <xsl:value-of select="."/>
  </title>
</xsl:template>

<xsl:template match="p">
  <p>
    <xsl:value-of select="."/>
  </p>
</xsl:template>

<xsl:template match="section">
  <xsl:apply-templates select="item"/>
</xsl:template>

<xsl:template match="item">
</xsl:template>

</xsl:stylesheet>
